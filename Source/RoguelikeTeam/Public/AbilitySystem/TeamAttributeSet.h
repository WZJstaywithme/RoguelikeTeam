// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"  // 添加这个头文件
#include "GameplayEffectExtension.h"  // 可能也需要这个
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "TeamAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// 属性类别枚举
UENUM(BlueprintType)
enum class EAttributeCategory : uint8
{
	Primary,
	Secondary,
	Resistance,
	Vital,
	Meta
};

// 属性信息结构体
USTRUCT(BlueprintType)
struct FTeamAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttributeCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	FTeamAttributeInfo() : AttributeName(NAME_None), Category(EAttributeCategory::Primary) {}
    
	FTeamAttributeInfo(FName InName, EAttributeCategory InCategory, FGameplayTag InTag)
		: AttributeName(InName), Category(InCategory), AttributeTag(InTag) {}
};

/**
 * 模板化优化的属性集类
 * 使用模板化设计大幅减少代码重复
 */
UCLASS()
class ROGUELIKETEAM_API UTeamAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UTeamAttributeSet();
    
    // 属性定义宏调用 - 大幅简化属性声明
    // Primary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Strength);
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Intelligence);
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Agile;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Agile);

    // Secondary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Attack);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Defense);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Critical;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Critical);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, CriticalDamage);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HitRate;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, HitRate);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Avoid;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Avoid);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Wsp;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Wsp);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, MaxHealth);
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, MaxMana);

    // Resistance Attributes
    // DEFINE_ATTRIBUTE(FireResistance, "Resistance Attributes");
    // DEFINE_ATTRIBUTE(LightningResistance, "Resistance Attributes");
    // DEFINE_ATTRIBUTE(ArcaneResistance, "Resistance Attributes");
    // DEFINE_ATTRIBUTE(PhysicalResistance, "Resistance Attributes");

    // Vital Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Health);
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, Mana);

    // Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, IncomingDamage);
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UTeamAttributeSet, IncomingXP);

    // 核心功能方法
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
    // 标签到属性的映射
    TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

private:
    // 私有辅助方法
    void HandleIncomingDamage(const FEffectProperties& Props);
    void HandleIncomingXP(const FEffectProperties& Props);
    void Debuff(const FEffectProperties& Props);
    void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
    void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
    void SendXPEvent(const FEffectProperties& Props);

    // 状态标志
    bool bTopOffHealth = false;
    bool bTopOffMana = false;

    // 初始化属性映射
    void InitializeAttributeMappings();
    FGameplayAttribute GetAttributeByTag(const FGameplayTag& Tag) const;
    float GetAttributeValueByTag(const FGameplayTag& Tag) const;
};
