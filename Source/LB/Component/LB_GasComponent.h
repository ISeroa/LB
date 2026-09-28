// LB_GasComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "LB_GasComponent.generated.h"

class UAbilitySystemComponent;
class ULB_AttributeSet;
class ALB_PlayerState;

DECLARE_MULTICAST_DELEGATE(FOnLBGasReady);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLBAttributeChanged, const FOnAttributeChangeData&);

UCLASS(ClassGroup = (LB), meta = (BlueprintSpawnableComponent))
class LB_API ULB_GasComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULB_GasComponent();

	// 플레이어용: PS가 가진 ASC를 연결 (몬스터용 InitFromSelf는 나중에 추가)
	void InitFromPlayerState(ALB_PlayerState* PS);

	UAbilitySystemComponent* GetASC() const { return ASC; }
	ULB_AttributeSet* GetAttributeSet() const { return AttributeSet; }
	bool IsReady() const { return ASC != nullptr; }

	FOnLBGasReady OnGasReady;                    // HUD 등이 구독
	FOnLBAttributeChanged OnAnyAttributeChanged; // 어떤 속성이든 바뀌면 호출

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BindDelegates();
	void UnbindDelegates();
	void HandleAttributeChanged(const FOnAttributeChangeData& Data);
	void ApplySpeedToMovement(float NewSpeed);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<ULB_AttributeSet> AttributeSet;

	TArray<TPair<FGameplayAttribute, FDelegateHandle>> Bindings;
};