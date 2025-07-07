// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PointCollection.h"
#include "Engine/DataAsset.h"
#include "FormationInfo.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UFormationInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FormationID;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FormationDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APointCollection> FormationPoints;
};
