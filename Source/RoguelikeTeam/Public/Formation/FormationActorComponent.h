// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TeamCharacter.h"
#include "Character/TeamPartner.h"
#include "Components/ActorComponent.h"
#include "Data/FormationInfo.h"
#include "FormationActorComponent.generated.h"

class APointCollection;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKETEAM_API UFormationActorComponent : public UActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UFormationActorComponent();
	void InitPoint(UFormationInfo* FormationInfo);
	void AddMemberToSlot(ATeamPartner* Partner, int32 SlotIndex);
	void RotateFormation(float Angle);
	void UpdateAllMemberPositions(FVector ActorLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APointCollection* PointCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

	TSubclassOf<APointCollection> PointCollectionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Leader = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFormationInfo* FormationInfo;

public:
	UPROPERTY(EditAnywhere)
	TMap<int32, ATeamPartner*> FormationCharacterMap;
};
