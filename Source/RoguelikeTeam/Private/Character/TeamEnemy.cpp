// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamEnemy.h"

#include "AbilitySystemComponent.h"
#include "TeamGameplayTags.h"
#include "AbilitySystem/TeamAbilitySystemComponent.h"
#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "AbilitySystem/TeamAttributeSet.h"
#include "AI/TeamAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RoguelikeTeam/RoguelikeTeam.h"

ATeamEnemy::ATeamEnemy()
{
}

void ATeamEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ATeamEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TeamAIController = Cast<ATeamAIController>(NewController);
	TeamAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	TeamAIController->RunBehaviorTree(BehaviorTree);
	TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

