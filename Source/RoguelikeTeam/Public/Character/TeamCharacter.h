// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TeamCharacterBase.h"
#include "Player/TeamPlayerController.h"
#include "TeamCharacter.generated.h"

class APartnerAIController;
class UBehaviorTree;
class UFormationInfo;
class UFormationActorComponent;
class ATeamPlayerState;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamCharacter : public ATeamCharacterBase
{
	GENERATED_BODY()

	ATeamCharacter();

	virtual void BeginPlay() override;
public:
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UFormationInfo> FormationInfo;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

protected:
	UPROPERTY()
	TObjectPtr<APartnerAIController> TeamAIController;
};
