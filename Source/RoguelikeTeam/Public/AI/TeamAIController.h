// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeamAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATeamAIController();
public:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	void SetMoveTarget(const FVector& NewTarget);
};
