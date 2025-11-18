// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/TeamGameplayAbility.h"
#include "ProjectileSpell.generated.h"

class ATeamProjectile;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UProjectileSpell : public UTeamDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATeamProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;
};
