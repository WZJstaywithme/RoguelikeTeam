// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EruptSpell.h"

#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

UEruptSpell::UEruptSpell()
{
	// 创建碰撞组件
	OverlapShape = CreateDefaultSubobject<UShapeComponent>(TEXT("OverlapShape"));
	if (!OverlapShape)
	{
		OverlapShape = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapShape"));
	}
	OverlapShape->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 默认禁用
	OverlapShape->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapShape->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

	// 绑定碰撞事件
	OverlapShape->OnComponentBeginOverlap.AddDynamic(this, &UEruptSpell::OnOverlapBegin);
	OverlapShape->OnComponentEndOverlap.AddDynamic(this, &UEruptSpell::OnOverlapEnd);
}

void UEruptSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor) return;

	// 清空之前的目标
	DetectedTargets.Empty();
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(AvatarActor);

	// 优化：只在必要时附加组件
	if (USceneComponent* RootComp = AvatarActor->GetRootComponent())
	{
		// 检查组件是否已经附加到正确的根组件
		if (OverlapShape->GetAttachParent() != RootComp)
		{
			// 只有当附加父对象不同时才重新附加
			OverlapShape->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	// 根据不同的形状类型设置尺寸
	if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(OverlapShape))
	{
		BoxComponent->SetBoxExtent(ShapeExtent);
		BoxComponent->SetRelativeLocation(FVector(ShapeExtent.X,0.f, 0.f));
	}
	else if (USphereComponent* SphereComponent = Cast<USphereComponent>(OverlapShape))
	{
		SphereComponent->SetSphereRadius(ShapeExtent.X); // 使用X分量作为半径
	}
	else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(OverlapShape))
	{
		CapsuleComponent->SetCapsuleRadius(ShapeExtent.X);
		CapsuleComponent->SetCapsuleHalfHeight(ShapeExtent.Y);
	}

	// 启用碰撞检测
	OverlapShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 每帧绘制调试形状
	if (bShowDebugShape && GetWorld())
	{
		// 创建定时器来持续绘制调试形状
		FTimerDelegate DebugDrawDelegate;
		DebugDrawDelegate.BindUObject(this, &UEruptSpell::DrawDebugShape);
		GetWorld()->GetTimerManager().SetTimer(DebugDrawTimerHandle, DebugDrawDelegate, 0.05f, true);
	}
}

void UEruptSpell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	// 检查是否是有效的目标
	if (!OtherActor || ActorsToIgnore.Contains(OtherActor))
	{
		return;
	}

	// 添加到检测到的目标列表
	if (!DetectedTargets.Contains(OtherActor))
	{
		DetectedTargets.Add(OtherActor);
	}
}

void UEruptSpell::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 从目标列表中移除
	DetectedTargets.Remove(OtherActor);
}

void UEruptSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                             bool bWasCancelled)
{
	// 停止调试绘制定时器
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DebugDrawTimerHandle);
	}

	// 禁用碰撞检测
	if (OverlapShape)
	{
		OverlapShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 清空目标列表
	DetectedTargets.Empty();
	ActorsToIgnore.Empty();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// 根据不同形状绘制调试形状
void UEruptSpell::DrawDebugShape()
{
	if (OverlapShape && bShowDebugShape && GetWorld())
	{
		// 根据形状类型绘制不同的调试几何体
		if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(OverlapShape))
		{
			DrawDebugBox(
				GetWorld(),
				BoxComponent->GetComponentLocation(),
				BoxComponent->GetScaledBoxExtent(),
				BoxComponent->GetComponentRotation().Quaternion(),
				DebugShapeColor,
				false,
				0.05f,
				0,
				DebugShapeThickness
			);
		}
		else if (USphereComponent* SphereComponent = Cast<USphereComponent>(OverlapShape))
		{
			DrawDebugSphere(
				GetWorld(),
				SphereComponent->GetComponentLocation(),
				SphereComponent->GetScaledSphereRadius(),
				20, // 球体分段数
				DebugShapeColor,
				false,
				0.05f,
				0,
				DebugShapeThickness
			);
		}
		else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(OverlapShape))
		{
			DrawDebugCapsule(
				GetWorld(),
				CapsuleComponent->GetComponentLocation(),
				CapsuleComponent->GetScaledCapsuleHalfHeight(),
				CapsuleComponent->GetScaledCapsuleRadius(),
				CapsuleComponent->GetComponentRotation().Quaternion(),
				DebugShapeColor,
				false,
				0.05f,
				0,
				DebugShapeThickness
			);
		}

		// 绘制检测到的目标数量
		FString DebugText = FString::Printf(TEXT("检测到 %d 个目标"), DetectedTargets.Num());
		DrawDebugString(
			GetWorld(),
			OverlapShape->GetComponentLocation() + FVector(0, 0, 100),
			DebugText,
			nullptr,
			FColor::White,
			0.05f,
			true
		);
	}
}
