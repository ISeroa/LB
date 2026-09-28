// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/LB_MonsterBase.h"

// Sets default values
ALB_MonsterBase::ALB_MonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALB_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALB_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALB_MonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

