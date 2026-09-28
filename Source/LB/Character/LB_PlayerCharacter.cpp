// Fill out your copyright notice in the Description page of Project Settings.


#include "LB_PlayerCharacter.h"

#include "Component/LB_GasComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerState/LB_PlayerState.h"

// Sets default values
ALB_PlayerCharacter::ALB_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GasComp = CreateDefaultSubobject<ULB_GasComponent>(TEXT("GasComp"));
	
}

UAbilitySystemComponent* ALB_PlayerCharacter::GetAbilitySystemComponent() const
{
	return GasComp ? GasComp->GetASC() : nullptr;
}

// Called when the game starts or when spawned
void ALB_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALB_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitGAS();
}

void ALB_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitGAS();
}

void ALB_PlayerCharacter::InitGAS()
{
	if (ALB_PlayerState* PS = GetPlayerState<ALB_PlayerState>())
	{
		GasComp->InitFromPlayerState(PS);
	}
}

// Called every frame
void ALB_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALB_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

