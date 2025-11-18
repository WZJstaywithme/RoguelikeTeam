// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TeamAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UTeamAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UTeamAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
