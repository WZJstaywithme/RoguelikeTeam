// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "AbilitySystem/TeamFunctionLibrary.h"
#include "Actor/TeamProjectile.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/CombatInterface.h"

class ATeamProjectile;

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
                                       bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (APlayerState* PS = Cast<APlayerState>(AvatarActor))
	{
		AvatarActor = PS->GetPawn();
	}
    
	
	if (!AvatarActor || !AvatarActor->Implements<UCombatInterface>())
	{
		return;
	}
	
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
	AvatarActor,
	SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UTeamFunctionLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		ATeamProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATeamProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		// Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
