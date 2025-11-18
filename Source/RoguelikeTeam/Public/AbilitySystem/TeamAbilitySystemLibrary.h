// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Character/TeamCombatCharacter.h"
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

UCLASS()
class ROGUELIKETEAM_API UTeamAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	                                        float Level, UAbilitySystemComponent* ASC);
	static void InitAbilitySystemUI(const UObject* WorldContextObject, ATeamCombatCharacter* Character,
	                                ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	                                 ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref)
	                            FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)
	                             FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsAvoidedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsAvoidedHit(UPARAM(ref)
	                            FGameplayEffectContextHandle& EffectContextHandle, bool bInIsAvoidedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref)
	                              FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref)
	                                       FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref)
	                                       FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref)
	                                  FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref)
	                              FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	                                       const TArray<AActor*>& ActorsToIgnore, float Radius,
	                                       const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayTas")
	static FGameplayTag FindTag(const FName& TagName);

	/**
	* 
	*/
	/** * 手动给 ASC 添加一个 Loose Tag (不会自动消失，除非手动移除)
		 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayTas")
	static void AddLooseTagToAsc(UAbilitySystemComponent* ASC, FGameplayTag Tag, int32 Count = 1);


	/** * 手动移除 ASC 的 Loose Tag
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayTas")
	static void RemoveLooseTagFromAsc(UAbilitySystemComponent* ASC, FGameplayTag Tag, int32 Count = 1);
};
