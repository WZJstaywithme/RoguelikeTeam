// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamCharacter.h"

#include "AI/PartnerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Formation/FormationActorComponent.h"
#include "Game/TeamGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TeamPlayerState.h"

class ATeamGameModeBase;
class ATeamPlayerState;

ATeamCharacter::ATeamCharacter()
{
	
}

void ATeamCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	// LoadProgress();

	// if (ATeamGameModeBase* AuraGameMode = Cast<ATeamGameModeBase>(UGameplayStatics::GetGameMode(this)))
	// {
	// 	AuraGameMode->LoadWorldState(GetWorld());
	// }
}

void ATeamCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATeamCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}


void ATeamCharacter::InitAbilityActorInfo()
{
	ATeamPlayerState* TeamPlayerState = GetPlayerState<ATeamPlayerState>();
	check(TeamPlayerState);
	TeamPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TeamPlayerState, this);
	// Cast<UTeamAbilitySystemComponent>(TeamPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = TeamPlayerState->GetAbilitySystemComponent();
	// AttributeSet = TeamPlayerState->GetAttributeSet();
	// OnAscRegistered.Broadcast(AbilitySystemComponent);
	// AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);
	//
	// if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	// {
	// 	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	// 	{
	// 		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	// 	}
	// }
}
