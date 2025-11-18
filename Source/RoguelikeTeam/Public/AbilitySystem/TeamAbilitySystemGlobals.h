// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "TeamAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UTeamAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
