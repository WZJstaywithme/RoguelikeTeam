// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TeamGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UTeamGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	float GetManaCost(float InLevel = 0.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
