// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BeamSpell.h"

#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UBeamSpell::StoreTargetDataInfo(AActor* CombatTarget)
{
	if (CombatTarget)
	{
		TargetHitLocation = CombatTarget->GetActorLocation();
		HitTargetActor = CombatTarget;
	}
}

void UBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				TargetHitLocation = HitResult.ImpactPoint;
				HitTargetActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitTargetActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamSpell::PrimaryTargetDied);
		}
	}
}

// void UBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
// {
// 	TArray<AActor*> ActorsToIgnore;
// 	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
// 	
// 	TArray<AActor*> OverlappingActors;
// 	UTeamAbilitySystemLibrary::GetLivePlayersWithinRadius(
// 		GetAvatarActorFromActorInfo(),
// 		OverlappingActors,
// 		ActorsToIgnore,
// 		850.f,
// 		MouseHitActor->GetActorLocation());
// 	
// 	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
// 	//int32 NumAdditionTargets = 5;
// 	
// 	UTeamAbilitySystemLibrary::GetClosestTargets(
// 		NumAdditionalTargets,
// 		OverlappingActors,
// 		OutAdditionalTargets,
// 		MouseHitActor->GetActorLocation());
//
// 	for (AActor* Target : OutAdditionalTargets)
// 	{
// 		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
// 		{
// 			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
// 			{
// 				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
// 			}
// 		}
// 	}
// }
