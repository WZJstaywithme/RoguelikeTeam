// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamPartner.h"

#include "AbilitySystem/TeamAbilitySystemComponent.h"
#include "AbilitySystem/TeamFunctionLibrary.h"
#include "AI/PartnerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

class APartnerAIController;

ATeamPartner::ATeamPartner()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ATeamPartner::BeginPlay()
{
	Super::BeginPlay();
}

void ATeamPartner::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

