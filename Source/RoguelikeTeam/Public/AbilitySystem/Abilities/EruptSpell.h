// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TeamDamageGameplayAbility.h"
#include "EruptSpell.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UEruptSpell : public UTeamDamageGameplayAbility
{
	GENERATED_BODY()

	UEruptSpell();
	
	void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                     FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	// 碰撞事件回调
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	void DrawDebugShape();

protected:
	
	// 使用UShapeComponent基类，支持任意形状
	UPROPERTY(EditAnywhere, Category = "Collision")
	class UShapeComponent* OverlapShape;

	// 通用的尺寸设置（根据不同形状有不同含义）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector ShapeExtent = FVector(200.f); // 对于球体，x分量作为半径；对于盒子，作为三维尺

	// 添加调试控制选项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugShape = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	FColor DebugShapeColor  = FColor(255, 0, 0, 128); // 半透明红色

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float DebugShapeThickness  = 2.0f;
	
	// 检测到的目标列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TArray<AActor*> DetectedTargets;

	// 忽略的Actor
	TArray<AActor*> ActorsToIgnore;

	// 调试绘制定时器句柄
	FTimerHandle DebugDrawTimerHandle;
};
