// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"

// 先引用 UE 核心头文件
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

// 再引用自定义头文件
#include "TeamGameplayTags.h"
#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "AbilitySystem/TeamAttributeSet.h"
#include "Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

class UCharacterClassInfo;
struct FTeamGameplayTags;

struct TeamDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HitRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Avoid);

	// DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TeamDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, Critical, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, CriticalDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, HitRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, Avoid, Target, false);

		// DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, FireResistance, Target, false);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, LightningResistance, Target, false);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, ArcaneResistance, Target, false);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(UTeamAttributeSet, PhysicalResistance, Target, false);
	}
};

static const TeamDamageStatics& DamageStatics()
{
	static TeamDamageStatics DStatics;
	return DStatics;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().HitRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().AvoidDef);

	// RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	// RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	// RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	// RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalcDamage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                      const FGameplayEffectSpec& Spec,
                                      FAggregatorEvaluateParameters EvaluationParameters,
                                      const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs)
const
{
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FTeamGameplayTags& Tags = FTeamGameplayTags::Get();

	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Attack, DamageStatics().AttackDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, DamageStatics().DefenseDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Critical, DamageStatics().CriticalDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalDamage, DamageStatics().CriticalDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_HitRate, DamageStatics().HitRateDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Avoid, DamageStatics().AvoidDef);

	// TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	// TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	// TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	// TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);


	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Debuff
	// DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);

	// Get Damage Set by Caller Magnitude
	// for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : FTeamGameplayTags::Get().DamageTypesToResistances)
	// {
	// 	const FGameplayTag DamageTypeTag = Pair.Key;
	// 	const FGameplayTag ResistanceTag = Pair.Value;
	// 	
	// 	checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
	// 	const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];
	//
	// 	float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
	// 	if (DamageTypeValue <= 0.f)
	// 	{
	// 		continue;
	// 	}
	// 	
	// 	float Resistance = 0.f;
	// 	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
	// 	Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
	//
	// 	DamageTypeValue *= ( 100.f - Resistance ) / 100.f;
	//
	// 	if (UTeamAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
	// 	{
	// 		// 1. override TakeDamage in TeamCharacterBase. *
	// 		// 2. create delegate OnDamageDelegate, broadcast damage received in TakeDamage *
	// 		// 3. Bind lambda to OnDamageDelegate on the Victim here. *
	// 		// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage (this will result in TakeDamage being called
	// 		//		on the Victim, which will then broadcast OnDamageDelegate)
	// 		// 5. In Lambda, set DamageTypeValue to the damage received from the broadcast *
	//
	// 		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
	// 		{
	// 			CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
	// 			{
	// 				DamageTypeValue = DamageAmount;
	// 			});
	// 		}
	// 		UGameplayStatics::ApplyRadialDamageWithFalloff(
	// 			TargetAvatar,
	// 			DamageTypeValue,
	// 			0.f,
	// 			UTeamAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
	// 			UTeamAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
	// 			UTeamAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
	// 			1.f,
	// 			UDamageType::StaticClass(),
	// 			TArray<AActor*>(),
	// 			SourceAvatar,
	// 			nullptr);
	// 	}
	// 	
	// 	Damage += DamageTypeValue;
	// }

	// Capture BlockChance on Target, and determine if there was a successful Block

	float TargetAvoidChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AvoidDef, EvaluationParameters,
	                                                           TargetAvoidChance);
	float SourceHitRateChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HitRateDef, EvaluationParameters,
	                                                           SourceHitRateChance);

	float HitRateChance = FMath::Clamp(SourceHitRateChance - TargetAvoidChance, 5.f, 95.f);
	const bool bAvoided = FMath::RandRange(0, 100) < HitRateChance;

	UTeamAbilitySystemLibrary::SetIsAvoidedHit(EffectContextHandle, bAvoided);

	float Damage = 0.f;
	if (bAvoided)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters,
																   Damage);

		float TargetDefense = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, TargetDefense);
		TargetDefense = FMath::Clamp(TargetDefense, 0.f, 75.f);
		
		// If Block, halve the damage.	
		Damage *= TargetDefense / 100.f;
		
		float SourceCriticalChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDef, EvaluationParameters,
																   SourceCriticalChance);
		
		const bool bCritical = FMath::RandRange(0, 100) < SourceCriticalChance;
		if (bCritical)
		{
			float SourceCriticalDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageDef, EvaluationParameters,
																	   SourceCriticalDamage);
			SourceCriticalDamage = FMath::Max<float>(SourceCriticalDamage, 110.f);
			Damage *= SourceCriticalDamage / 100.f;
			UTeamAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCritical);
		}
	}

	const FGameplayModifierEvaluatedData EvaluatedData(UTeamAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
