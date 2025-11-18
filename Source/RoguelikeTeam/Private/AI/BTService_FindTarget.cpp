// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindTarget.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Character/TeamCharacterBase.h"
#include "Engine/OverlapResult.h"


void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();
	if (!IsValid(OwningPawn)) return;

	ATeamCharacterBase* TeamCharacter = CastChecked<ATeamCharacterBase>(OwningPawn);

	const FName TargetTag = OwningPawn -> ActorHasTag(FName("Team")) ? FName("Enemy") : FName("Team");
	const float SearchRadius =TeamCharacter -> AttackRange;	

	FVector Pos = UBTFunctionLibrary::GetBlackboardValueAsVector(this, PosLocationSelector);

	// 添加调试绘制 - 显示检测球体
	DrawDebugCircle(GetWorld(), Pos, SearchRadius, 32, FColor::Green, false, 0.5f,0,     // 优先级
		1.0f,  // 线宽
		FVector(1, 0, 0), // X轴
		FVector(0, 1, 0), // Y轴
		false  // 不绘制Z轴
	);
	
	// 使用球体检搜索目标
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningPawn);
	
	if (GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Pos,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams))
	{
		float ClosestDistance = TNumericLimits<float>::Max();
		AActor* ClosestActor = nullptr;
		
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* Actor = Result.GetActor();
			if (IsValid(Actor) && Actor->ActorHasTag(TargetTag))
			{
				const float Distance = OwningPawn->GetDistanceTo(Actor);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestActor = Actor;
				}
			}
		}

		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
	}
}