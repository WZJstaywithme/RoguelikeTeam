#pragma once

#include "GameplayEffectTypes.h"
#include "Character/TeamCharacterBase.h"
#include "TeamAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

#define DECLARE_GETTER(PropName, ReturnType) \
ReturnType Get##PropName() const { return PropName; }

#define DECLARE_SETTER(PropName, ParamType) \
void Set##PropName(ParamType In##PropName) { PropName = In##PropName; }

// 修正 bool 类型 GET 宏：成员变量带 b 前缀，方法返回 bPropName
#define DECLARE_BOOL_GETTER(PropName) \
bool Get##PropName() const { return b##PropName; }

// 修正 bool 类型 SET 宏：参数前缀 bIn，赋值给 bPropName
#define DECLARE_BOOL_SETTER(PropName) \
void Set##PropName(bool bIn##PropName) { b##PropName = bIn##PropName; }

// 普通类型 GET/SET 组合宏
#define DECLARE_GETTER_SETTER(PropName, ReturnType, ParamType) \
DECLARE_GETTER(PropName, ReturnType) \
DECLARE_SETTER(PropName, ParamType)

// bool 类型 GET/SET 组合宏（匹配 b 前缀成员变量）
#define DECLARE_BOOL_GETTER_SETTER(PropName) \
DECLARE_BOOL_GETTER(PropName) \
DECLARE_BOOL_SETTER(PropName)

USTRUCT(BlueprintType)
struct FTeamGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	DECLARE_BOOL_GETTER_SETTER(IsAvoidedHit)
	DECLARE_BOOL_GETTER_SETTER(IsCriticalHit)
	DECLARE_BOOL_GETTER_SETTER(IsSuccessfulDebuff)
	DECLARE_GETTER_SETTER(DebuffDamage, float, float)
	DECLARE_GETTER_SETTER(DebuffDuration, float, float)
	DECLARE_GETTER_SETTER(DebuffFrequency, float, float)
	DECLARE_GETTER_SETTER(DamageType, TSharedPtr<FGameplayTag>, TSharedPtr<FGameplayTag>)
	DECLARE_GETTER_SETTER(DeathImpulse, FVector, const FVector&)
	DECLARE_GETTER_SETTER(KnockbackForce, FVector, const FVector&)
	DECLARE_BOOL_GETTER_SETTER(IsRadialDamage)
	DECLARE_GETTER_SETTER(RadialDamageInnerRadius, float, float)
	DECLARE_GETTER_SETTER(RadialDamageOuterRadius, float, float)
	DECLARE_GETTER_SETTER(RadialDamageOrigin, FVector, const FVector&)

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

protected:
	UPROPERTY()
	bool bIsAvoidedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template <>
struct TStructOpsTypeTraits<FTeamGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTeamGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = false,
		WithCopy = true
	};
};
