// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TeamDamageGameplayAbility.h"
#include "BeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UBeamSpell : public UTeamDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreTargetDataInfo(AActor* CombatTarget);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	// UFUNCTION(BlueprintCallable)
	// void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector TargetHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> HitTargetActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;
};
