// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TeamAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TeamGameplayTags.h"
#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Player/TeamPlayerController.h"
enum class ECharacterClass : uint8;
class ATeamPlayerController;

UTeamAttributeSet::UTeamAttributeSet()
{
	InitializeAttributeMappings();
}

void UTeamAttributeSet::InitializeAttributeMappings()
{
	const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();

	// 主属性映射
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Agile, GetAgileAttribute);

	// 次要属性映射
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Attack, GetAttackAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Defense, GetDefenseAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Critical, GetCriticalAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalDamage, GetCriticalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HitRate, GetHitRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Avoid, GetAvoidAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Wsp, GetWspAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ActionRate, GetActionRateAttribute);
    
	// 抗性属性映射
	// TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

FGameplayAttribute UTeamAttributeSet::GetAttributeByTag(const FGameplayTag& Tag) const
{
    if (const TStaticFuncPtr<FGameplayAttribute()>* GetterPtr = TagsToAttributes.Find(Tag))
    {
        return (*GetterPtr)();
    }
    return FGameplayAttribute();
}

float UTeamAttributeSet::GetAttributeValueByTag(const FGameplayTag& Tag) const
{
    FGameplayAttribute Attribute = GetAttributeByTag(Tag);
    if (Attribute.IsValid())
    {
        return Attribute.GetNumericValue(this);
    }
    return 0.0f;
}

void UTeamAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }
    if (Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
    }
}

void UTeamAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
    // Source = causer of the effect, Target = target of the effect (owner of this AS)

    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

    if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
        Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
        if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
        {
            if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
            {
                Props.SourceController = Pawn->GetController();
            }
        }
        if (Props.SourceController)
        {
            Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
        }
    }

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
        Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
    }
}

void UTeamAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    
    FEffectProperties Props;
    SetEffectProperties(Data, Props);

    // if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        HandleIncomingDamage(Props);
    }
    if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
    {
        HandleIncomingXP(Props);
    }
}

void UTeamAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
    const float LocalIncomingDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    if (LocalIncomingDamage > 0.f)
    { 
        const float NewHealth = GetHealth() - LocalIncomingDamage;
        SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
    
        const bool bFatal = NewHealth <= 0.f;
        if (bFatal)
        {
            // ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
            // if (CombatInterface)
            // {
            //     FVector Impulse = UTeamAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
            //     CombatInterface->Die(UTeamAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
            // }
            // SendXPEvent(Props);
            
        }
        else
        {
            // if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
            // {
            //     FGameplayTagContainer TagContainer;
            //     TagContainer.AddTag(FTeamGameplayTags::Get().Effects_HitReact);
            //     Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
            // }
            
            // const FVector& KnockbackForce = UTeamAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
            // if (!KnockbackForce.IsNearlyZero(1.f))
            // {
            //     Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
            // }
        }
            
        const bool bBlock = UTeamAbilitySystemLibrary::IsAvoidedHit(Props.EffectContextHandle);
        const bool bCriticalHit = UTeamAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
        ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
        // if (UTeamAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
        // {
        //     Debuff(Props);
        // }
    }
}

void UTeamAttributeSet::Debuff(const FEffectProperties& Props)
{
    const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();
    FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
    EffectContext.AddSourceObject(Props.SourceAvatarActor);

    // const FGameplayTag DamageType = UTeamAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
    // const float DebuffDamage = UTeamAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
    // const float DebuffDuration = UTeamAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
    // const float DebuffFrequency = UTeamAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

    // FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
    // UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
    //
    // Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    // Effect->Period = DebuffFrequency;
    // Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
    //
    // const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
    // Effect->InheritableOwnedTagsContainer.AddTag(DebuffTag);
    // if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
    // {
    //     Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
    //     Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputHeld);
    //     Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputPressed);
    //     Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputReleased);
    // }
    //
    // Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
    // Effect->StackLimitCount = 1;
    //
    // const int32 Index = Effect->Modifiers.Num();
    // Effect->Modifiers.Add(FGameplayModifierInfo());
    // FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
    //
    // ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
    // ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    // ModifierInfo.Attribute = GetIncomingDamageAttribute();
    
    // if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
    // {
        // FTeamGameplayEffectContext* TeamContext = static_cast<FTeamGameplayEffectContext*>(MutableSpec->GetContext().Get());
        // TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
        // TeamContext->SetDamageType(DebuffDamageType);

        // Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
    // }
}

void UTeamAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
    const float LocalIncomingXP = GetIncomingXP();
    SetIncomingXP(0.f);

    // Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
    // if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
    // {
    //     const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
    //     const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
    //
    //     const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
    //     const int32 NumLevelUps = NewLevel - CurrentLevel;
    //     if (NumLevelUps > 0)
    //     {
    //         IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
    //
    //         int32 AttributePointsReward = 0;
    //         int32 SpellPointsReward = 0;
    //
    //         for (int32 i = 0; i < NumLevelUps; ++i)
    //         {
    //             SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
    //             AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
    //         }
    //         
    //         IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
    //         IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);
    //
    //         bTopOffHealth = true;
    //         bTopOffMana = true;
    //             
    //         IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
    //     }
    //         
    //     IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
    // }
}

void UTeamAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
    {
        SetHealth(GetMaxHealth());
        bTopOffHealth = false;
    }
    if (Attribute == GetMaxManaAttribute() && bTopOffMana)
    {
        SetMana(GetMaxMana());
        bTopOffMana = false;
    }
    if (Attribute == GetActionAttribute() && Attribute.GetNumericValue(this) > 100.f)
    {
        if (Attribute.GetNumericValue(this) != 100.f)
        {
            SetAction(100.f);
        }
    }
}

void UTeamAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
    if (Props.TargetCharacter->Implements<UCombatInterface>())
    {
        // const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
        // const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
        // const int32 XPReward = UTeamAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

        // const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();
        // FGameplayEventData Payload;
        // Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
        // Payload.EventMagnitude = XPReward;
        // UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
    }
}

void UTeamAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
    if (!IsValid(Props.SourceCharacter) || !IsValid(Props.TargetCharacter)) return;
    if (Props.SourceCharacter != Props.TargetCharacter)
    {
        if(ATeamPlayerController* PC = Cast<ATeamPlayerController>(Props.SourceCharacter->Controller))
        {
            // PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
            return;
        }
        if(ATeamPlayerController* PC = Cast<ATeamPlayerController>(Props.TargetCharacter->Controller))
        {
            // PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
        }
    }
}