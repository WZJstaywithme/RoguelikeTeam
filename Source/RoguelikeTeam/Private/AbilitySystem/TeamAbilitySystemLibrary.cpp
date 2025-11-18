// // Copyright Druid Mechanics
//
//
#include "AbilitySystem/TeamAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TeamGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TeamAbilityTypes.h"
#include "TeamGameplayTags.h"
#include "Game/TeamGameModeBase.h"
// #include "Game/LoadScreenSaveGame.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TeamPlayerState.h"
// #include "UI/HUD/TeamHUD.h"
// #include "UI/WidgetController/TeamWidgetController.h"

#define IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(ClassName, PropertyName, ValueType, DefaultValue) \
void ClassName::Set##PropertyName(FGameplayEffectContextHandle& EffectContextHandle, ValueType Value) \
{ \
if (FTeamGameplayEffectContext* Context = static_cast<FTeamGameplayEffectContext*>(EffectContextHandle.Get())) \
{ \
Context->Set##PropertyName(Value); \
} \
} \
ValueType ClassName::Get##PropertyName(const FGameplayEffectContextHandle& EffectContextHandle) \
{ \
if (const FTeamGameplayEffectContext* Context = static_cast<const FTeamGameplayEffectContext*>(EffectContextHandle.Get())) \
{ \
return Context->Get##PropertyName(); \
} \
return DefaultValue; \
}

IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, IsCriticalHit, bool, false)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, IsRadialDamage, bool, false)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, IsAvoidedHit, bool, false)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, RadialDamageInnerRadius, float, 0.f)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, RadialDamageOuterRadius, float, 0.f)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, RadialDamageOrigin, FVector, FVector::ZeroVector)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, DeathImpulse, FVector, FVector::ZeroVector)
IMPLEMENT_EFFECT_CONTEXT_ACCESSORS(UTeamAbilitySystemLibrary, KnockbackForce, FVector, FVector::ZeroVector)


// bool UTeamAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ATeamHUD*& OutTeamHUD)
// {
// 	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
// 	{
// 		OutTeamHUD = Cast<ATeamHUD>(PC->GetHUD());
// 		if (OutTeamHUD)
// 		{
// 			ATeamPlayerState* PS = PC->GetPlayerState<ATeamPlayerState>();
// 			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
// 			UAttributeSet* AS = PS->GetAttributeSet();
//
// 			OutWCParams.AttributeSet = AS;
// 			OutWCParams.AbilitySystemComponent = ASC;
// 			OutWCParams.PlayerState = PS;
// 			OutWCParams.PlayerController = PC;
// 			return true;
// 		}
// 	}
// 	return false;
// }
//
// UOverlayWidgetController* UTeamAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
// {
// 	FWidgetControllerParams WCParams;
// 	ATeamHUD* TeamHUD = nullptr;
// 	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TeamHUD))
// 	{
// 		return TeamHUD->GetOverlayWidgetController(WCParams);
// 	}
// 	return nullptr;
// }
//
// UAttributeMenuWidgetController* UTeamAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
// {
// 	FWidgetControllerParams WCParams;
// 	ATeamHUD* TeamHUD = nullptr;
// 	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TeamHUD))
// 	{
// 		return TeamHUD->GetAttributeMenuWidgetController(WCParams);
// 	}
// 	return nullptr;
// }
//
// USpellMenuWidgetController* c::GetSpellMenuWidgetController(const UObject* WorldContextObject)
// {
// 	FWidgetControllerParams WCParams;
// 	ATeamHUD* TeamHUD = nullptr;
// 	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TeamHUD))
// 	{
// 		return TeamHUD->GetSpellMenuWidgetController(WCParams);
// 	}
// 	return nullptr;
// }
//
void UTeamAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

// void UTeamAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
// {
// 	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
// 	if (CharacterClassInfo == nullptr) return;
//
// 	const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();
//
// 	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
//
// 	FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
// 	EffectContexthandle.AddSourceObject(SourceAvatarActor);
//
// 	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContexthandle);
//
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);
//
// 	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
//
// 	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
// 	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
// 	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
// 	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
//
// 	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
// 	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
// 	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
// 	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
// }
//
void UTeamAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

// int32 UTeamAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
// {
// 	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
// 	if (CharacterClassInfo == nullptr) return 0;
//
// 	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
// 	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
//
// 	return static_cast<int32>(XPReward);
// }
//
// void UTeamAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
// {
// 	DamageEffectParams.bIsRadialDamage = bIsRadial;
// 	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
// 	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
// 	DamageEffectParams.RadialDamageOrigin = Origin;
// }
//
// void UTeamAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
// {
// 	KnockbackDirection.Normalize();
// 	if (Magnitude == 0.f)
// 	{
// 		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
// 	}
// 	else
// 	{
// 		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
// 	}
// }
//
// void UTeamAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
// {
// 	ImpulseDirection.Normalize();
// 	if (Magnitude == 0.f)
// 	{
// 		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
// 	}
// 	else
// 	{
// 		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
// 	}
// }
//
// void UTeamAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams,
// 	UAbilitySystemComponent* InASC)
// {
// 	DamageEffectParams.TargetAbilitySystemComponent = InASC;
// }

UCharacterClassInfo* UTeamAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ATeamGameModeBase* TeamGameMode = Cast<ATeamGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TeamGameMode == nullptr) return nullptr;
	return TeamGameMode->CharacterClassInfo;
}

// void UTeamAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
//                                                            TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
//                                                            const FVector& SphereOrigin)
// {
// 	FCollisionQueryParams SphereParams;
// 	SphereParams.AddIgnoredActors(ActorsToIgnore);
// 	
// 	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
// 	{
// 		TArray<FOverlapResult> Overlaps;
// 		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
// 		for (FOverlapResult& Overlap : Overlaps)
// 		{
// 			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
// 			{
// 				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
// 			}
// 		}
// 	}
// }
//
// void UTeamAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
// {
// 	if (Actors.Num() <= MaxTargets)
// 	{
// 		OutClosestTargets = Actors;
// 		return;
// 	}
//
// 	TArray<AActor*> ActorsToCheck = Actors;
// 	int32 NumTargetsFound = 0;
//
// 	while (NumTargetsFound < MaxTargets)
// 	{
// 		if (ActorsToCheck.Num() == 0) break;
// 		double ClosestDistance = TNumericLimits<double>::Max();
// 		AActor* ClosestActor;
// 		for (AActor* PotentialTarget : ActorsToCheck)
// 		{
// 			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
// 			if (Distance < ClosestDistance)
// 			{
// 				ClosestDistance = Distance;
// 				ClosestActor = PotentialTarget;
// 			}
// 		}
// 		ActorsToCheck.Remove(ClosestActor);
// 		OutClosestTargets.AddUnique(ClosestActor);
// 		++NumTargetsFound;
// 	}
// }
//
bool UTeamAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle UTeamAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

// TArray<FRotator> UTeamAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
// {
// 	TArray<FRotator> Rotators;
// 	
// 	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
// 	if (NumRotators > 1)
// 	{
// 		const float DeltaSpread = Spread / (NumRotators - 1);
// 		for (int32 i = 0; i < NumRotators; i++)
// 		{
// 			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
// 			Rotators.Add(Direction.Rotation());
// 		}
// 	}
// 	else
// 	{
// 		Rotators.Add(Forward.Rotation());
// 	}
// 	return Rotators;
// }
//
// TArray<FVector> UTeamAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
// {
// 	TArray<FVector> Vectors;
// 	
// 	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
// 	if (NumVectors > 1)
// 	{
// 		const float DeltaSpread = Spread / (NumVectors - 1);
// 		for (int32 i = 0; i < NumVectors; i++)
// 		{
// 			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
// 			Vectors.Add(Direction);
// 		}
// 	}
// 	else
// 	{
// 		Vectors.Add(Forward);
// 	}
// 	return Vectors;
// }