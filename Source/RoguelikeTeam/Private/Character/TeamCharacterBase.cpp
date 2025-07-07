// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamCharacterBase.h"

// Sets default values
ATeamCharacterBase::ATeamCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeamCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATeamCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

