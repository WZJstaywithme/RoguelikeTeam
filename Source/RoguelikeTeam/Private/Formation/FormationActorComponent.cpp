// Fill out your copyright notice in the Description page of Project Settings.


#include "Formation/FormationActorComponent.h"

#include "AI/TeamAIController.h"
#include "Kismet/KismetMathLibrary.h"

void UFormationActorComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	Actor->SetRootComponent(RootSceneComponent);
}

// Sets default values for this component's properties
UFormationActorComponent::UFormationActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComp");
}


// Called when the game starts
void UFormationActorComponent::InitPoint(UFormationInfo* InFormationInfo)
{
	FormationInfo = InFormationInfo;
	if (InFormationInfo && InFormationInfo->FormationPoints)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		PointCollection = GetWorld()->SpawnActor<APointCollection>(
			InFormationInfo->FormationPoints,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		// PointCollection->AttachToComponent(RootSceneComponent, FAttachmentTransformRules(
		// 	                                   EAttachmentRule::KeepWorld, // 位置规则
		// 	                                   EAttachmentRule::KeepWorld, // 旋转规则 - 保持世界旋转
		// 	                                   EAttachmentRule::KeepWorld, // 缩放规则
		// 	                                   false // 不焊接
		//                                    ));
	}
}

void UFormationActorComponent::AddMemberToSlot(ATeamPartner* Partner, int32 SlotIndex)
{
	if (Partner && PointCollection && SlotIndex >= 0 && SlotIndex < PointCollection->NumPoints)
	{
		FormationCharacterMap.Add(SlotIndex, Partner);
	}
}

void UFormationActorComponent::RotateFormation(float Angle)
{
	if (!PointCollection) return;

	PointCollection->RotatePoints(Angle);
}

void UFormationActorComponent::UpdateAllMemberPositions(FVector LeaderLocation)
{
	if (!PointCollection) return;

	for (const auto& Pair : FormationCharacterMap)
	{
		if (Pair.Key == 0)
		{
			continue;
		}
		if (ATeamPartner* Partner = Pair.Value)
		{
			FVector TargetPosition = (PointCollection->ImmutablePts[Pair.Key]->GetComponentLocation() -
				PointCollection->ImmutablePts[0]->GetComponentLocation());

			// 通知AI控制器
			if (ATeamAIController* AIController = Cast<ATeamAIController>(Partner->GetController()))
			{
				AIController->SetMoveTarget(LeaderLocation + TargetPosition);
			}
		}
	}
}
