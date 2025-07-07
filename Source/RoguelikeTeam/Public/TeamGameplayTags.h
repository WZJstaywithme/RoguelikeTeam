#pragma once
#include "GameplayTagContainer.h"

struct FTeamGameplayTags
{
public:
	static const FTeamGameplayTags& Get() { return GameplayTags;}

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
private:
	static FTeamGameplayTags GameplayTags;
};
