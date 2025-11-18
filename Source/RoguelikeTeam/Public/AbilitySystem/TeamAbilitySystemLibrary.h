// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Character/TeamEnemy.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "TeamAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class ULootTiers;
class ULoadScreenSaveGame;
class UAbilityInfo;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
struct FWidgetControllerParams;


#define DECLARE_EFFECT_CONTEXT_ACCESSORS(PropertyName, ValueType) \
UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects") \
static void Set##PropertyName(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, ValueType Value); \
UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects") \
static ValueType Get##PropertyName(const FGameplayEffectContextHandle& EffectContextHandle);


/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API UTeamAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:


	// 批量声明需要的参数（新增参数只需加一行）
	DECLARE_EFFECT_CONTEXT_ACCESSORS(IsCriticalHit, bool)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(IsRadialDamage, bool)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(IsAvoidedHit, bool)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(RadialDamageInnerRadius, float)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(RadialDamageOuterRadius, float)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(RadialDamageOrigin, FVector)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(DeathImpulse, FVector)
	DECLARE_EFFECT_CONTEXT_ACCESSORS(KnockbackForce, FVector)
	
//
// 	/*
// 	 * Widget Controller
// 	 */
//
// 	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
// 	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);
// 	
// 	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
// 	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
//
// 	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
// 	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
//
// 	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
// 	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
//
// 	/*
// 	 * Ability System Class Defaults
// 	 */
//
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

// 	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
// 	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame);
// 	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

// 	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
// 	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
//
// 	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
// 	static ULootTiers* GetLootTiers(const UObject* WorldContextObject);
// 	
// 	/*
// 	 * Effect Context Getters
// 	 */
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
// 	
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
// 	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

// 	/*
// 	 * Gameplay Mechanics
// 	 */
// 	
// 	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
// 	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
//
// 	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
// 	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
// 	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
// 	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
//
// 	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
// 	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
//
// 	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
//
	/*
	 * Damage Effect Params
	 */

	// UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	// static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);
	//
	// UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	// static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);
	//
	// UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	// static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);
	//
	// UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	// static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
};