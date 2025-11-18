// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TeamAbilitySystemGlobals.h"

#include "TeamAbilityTypes.h"

FGameplayEffectContext* UTeamAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTeamGameplayEffectContext();
}
