// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TeamAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TeamCharacter.h"

ATeamAIController::ATeamAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}

void ATeamAIController::SetMoveTarget(const FVector& NewTarget)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsVector("MoveToLocation", NewTarget);
	}
}