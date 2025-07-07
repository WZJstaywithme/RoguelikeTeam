// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamCharacter.h"

#include "AI/PartnerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Formation/FormationActorComponent.h"
#include "Player/TeamPlayerState.h"

class ATeamPlayerState;

ATeamCharacter::ATeamCharacter()
{
}

void ATeamCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APartnerAIController* AIController = Cast<APartnerAIController>(NewController))
	{
		TeamAIController = AIController;
		if (BehaviorTree && BehaviorTree->BlackboardAsset)
		{
			TeamAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
			TeamAIController->RunBehaviorTree(BehaviorTree);
		}
	}
}
	

void ATeamCharacter::BeginPlay()
{
	Super::BeginPlay();
}
