// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TeamAIController.h"
#include "AbilitySystemInterface.h"
#include "Character/TeamCharacter.h"
#include "GameFramework/PlayerState.h"
#include "TeamPlayerState.generated.h"

class ATeamPartner;
class UFormationInfo;
class UFormationActorComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere)
	int32 XP = 0;

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	
	ATeamPlayerState();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	UFormationActorComponent* GetFormationActorComponent() const;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFormationActorComponent> FormationActorComponent;

	UPROPERTY(EditAnywhere)
	TMap<int, TSubclassOf<ATeamPartner>> FormationCharacterMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeamAIController> TeamAIController;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void SpawnFormationTeam(ATeamCharacter* TeamCharacter);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
