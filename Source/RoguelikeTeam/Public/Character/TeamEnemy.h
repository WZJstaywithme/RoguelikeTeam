// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamCombatCharacter.h"
#include "Character/TeamCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "TeamEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ATeamAIController;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamEnemy : public ATeamCombatCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ATeamEnemy();
protected:
	void BeginPlay() override;
	
};
