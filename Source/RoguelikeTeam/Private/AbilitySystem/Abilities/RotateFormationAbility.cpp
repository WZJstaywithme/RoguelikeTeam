// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RotateFormationAbility.h"

#include "Formation/FormationActorComponent.h"
#include "Player/TeamPlayerState.h"

class UFormationActorComponent;
class ATeamPlayerState;

// void URotateFormationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
// {
// 	bool bClockwise = false;
// 	if (TriggerEventData)
// 	{
// 		bClockwise = TriggerEventData->EventMagnitude > 0;
// 	}
//     
// 	RotateFormation(bClockwise);
// }

// void URotateFormationAbility::RotateFormation(bool bClockwise)
// {
// 	if (ATeamPlayerState* PS = Cast<ATeamPlayerState>(GetActorInfo().PlayerController->PlayerState))
// 	{
// 		if(UFormationActorComponent* Formation = PS->GetFormationActorComponent())
// 		{
// 			float Angle = bClockwise ? 45.0f : -45.0f;
// 			Formation->RotateFormation(Angle);
//             
// 			if (APawn* ControlledPawn = GetActorInfo().PlayerController->GetPawn())
// 			{
// 				Formation->UpdateAllMemberPositions(ControlledPawn->GetActorLocation());
// 			}
// 		}
// 	}
// }