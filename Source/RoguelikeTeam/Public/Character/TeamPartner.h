// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamCombatCharacter.h"
#include "Character/TeamCharacterBase.h"
#include "TeamPartner.generated.h"

class UBehaviorTree;
class APartnerAIController;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamPartner : public ATeamCombatCharacter
{
	GENERATED_BODY()
public:
	ATeamPartner();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};
