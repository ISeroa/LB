// LB_GasComponent.cpp
#include "LB_GasComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/LB_AttributeSet.h"
#include "PlayerState/LB_PlayerState.h"

ULB_GasComponent::ULB_GasComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULB_GasComponent::InitFromPlayerState(ALB_PlayerState* PS)
{
	if (!PS || !PS->GetAbilitySystemComponent())
		return;

	UnbindDelegates(); // 여러 번 불려도 안전하게

	ASC = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();

	ASC->InitAbilityActorInfo(PS, GetOwner());
	BindDelegates();

	if (GetOwner()->HasAuthority())
	{
		PS->InitializePlayerDA(); // PS 안에서 1회만 실행됨
	}

	// 클라는 속성값이 늦게 복제되니 0이면 건너뛰고, 이후엔 델리게이트가 처리
	if (AttributeSet && AttributeSet->GetSpeed() > 0.f)
	{
		ApplySpeedToMovement(AttributeSet->GetSpeed());
	}

	OnGasReady.Broadcast();
}

void ULB_GasComponent::BindDelegates()
{
	if (!ASC || !AttributeSet)
		return;

	// AttributeSet의 FGameplayAttributeData 프로퍼티를 전부 바인딩 
	for (TFieldIterator<FStructProperty> It(AttributeSet->GetClass()); It; ++It)
	{
		if (It->Struct != FGameplayAttributeData::StaticStruct())
			continue;

		const FGameplayAttribute Attr(*It);
		FDelegateHandle Handle = ASC->GetGameplayAttributeValueChangeDelegate(Attr)
			.AddUObject(this, &ULB_GasComponent::HandleAttributeChanged);
		Bindings.Emplace(Attr, Handle);
	}
}

void ULB_GasComponent::UnbindDelegates()
{
	if (IsValid(ASC))
	{
		for (auto& B : Bindings)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(B.Key).Remove(B.Value);
		}
	}
	Bindings.Reset();
}

void ULB_GasComponent::HandleAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == ULB_AttributeSet::GetSpeedAttribute())
	{
		ApplySpeedToMovement(Data.NewValue);
	}
	OnAnyAttributeChanged.Broadcast(Data);
}

void ULB_GasComponent::ApplySpeedToMovement(float NewSpeed)
{
	if (const ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = NewSpeed;
		}
	}
}

void ULB_GasComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindDelegates();
	Super::EndPlay(EndPlayReason);
}