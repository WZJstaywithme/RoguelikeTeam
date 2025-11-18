// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TeamGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	ATeamGameModeBase();
    
	// 获取当前目标位置
	UFUNCTION(BlueprintPure)
	FVector GetMoveTarget() const { return CurrentMoveTarget; }
    
protected:
	// 当前移动目标
	FVector CurrentMoveTarget;
    
	// 目标公差
	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 100.0f;
};
