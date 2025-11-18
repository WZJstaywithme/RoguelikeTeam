// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TeamAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UTeamAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	// 输入控制
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
};
