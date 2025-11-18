// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/TeamWidgetController.h"

void UTeamWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTeamWidgetController::BroadcastInitialValues()
{
}

void UTeamWidgetController::BindCallbacksToDependencies()
{
}

ATeamPlayerController* UTeamWidgetController::GetTeamPC()
{
	if (TeamPlayerController == nullptr)
	{
		TeamPlayerController = Cast<ATeamPlayerController>(PlayerController);
	}
	return TeamPlayerController;
}

ATeamPlayerState* UTeamWidgetController::GetTeamPS()
{
	if (TeamPlayerState == nullptr)
	{
		TeamPlayerState = Cast<ATeamPlayerState>(PlayerState);
	}
	return TeamPlayerState;
}

UTeamAbilitySystemComponent* UTeamWidgetController::GetTeamASC()
{
	if (TeamAbilitySystemComponent == nullptr)
	{
		TeamAbilitySystemComponent = Cast<UTeamAbilitySystemComponent>(AbilitySystemComponent);
	}
	return TeamAbilitySystemComponent;
}

UTeamAttributeSet* UTeamWidgetController::GetTeamAS()
{
	if (TeamAttributeSet == nullptr)
	{
		TeamAttributeSet = Cast<UTeamAttributeSet>(AttributeSet);
	}
	return TeamAttributeSet;
}
