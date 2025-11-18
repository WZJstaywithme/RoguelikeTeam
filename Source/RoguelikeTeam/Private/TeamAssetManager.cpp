// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamAssetManager.h"

#include "TeamGameplayTags.h"
#include "AbilitySystemGlobals.h"

UTeamAssetManager& UTeamAssetManager::Get()
{
	check(GEngine);
	
	UTeamAssetManager* AuraAssetManager = Cast<UTeamAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UTeamAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FTeamGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}