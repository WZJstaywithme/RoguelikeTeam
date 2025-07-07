// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TeamAIController.h"
#include "Character/TeamCharacter.h"
#include "GameFramework/PlayerState.h"
#include "TeamPlayerState.generated.h"

class UFormationInfo;
class UFormationActorComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATeamPlayerState();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	UFormationActorComponent* GetFormationActorComponent() const;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFormationActorComponent> FormationActorComponent;

	UPROPERTY(EditAnywhere)
	TMap<int, TSubclassOf<ATeamCharacter>> FormationCharacterMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeamAIController> TeamAIController;

	void SpawnFormationTeam(ATeamCharacter* TeamCharacter);
};
