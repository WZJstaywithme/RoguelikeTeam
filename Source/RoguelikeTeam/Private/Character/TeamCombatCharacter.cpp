// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamCombatCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TeamGameplayTags.h"
#include "TeamUserWidget.h"
#include "AbilitySystem/TeamAbilitySystemComponent.h"
#include "AbilitySystem/TeamAbilitySystemLibrary.h"
#include "AbilitySystem/TeamAttributeSet.h"
#include "AI/TeamAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RoguelikeTeam/RoguelikeTeam.h"

ATeamCombatCharacter::ATeamCombatCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTeamAbilitySystemComponent>("AbilitySystemComponent");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UTeamAttributeSet>("AttributeSet");

	BarContainer = CreateDefaultSubobject<UWidgetComponent>("BarContainer");
	BarContainer->SetupAttachment(GetRootComponent());

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	if (IsValid(Weapon))
	{
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
		Weapon->MarkRenderStateDirty();
	}

	BaseWalkSpeed = 500.f;
}

void ATeamCombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UTeamAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	UTeamAbilitySystemLibrary::InitAbilitySystemUI(this, this, CharacterClass);
	if (UTeamUserWidget* AuraUserWidget = Cast<UTeamUserWidget>(BarContainer->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UTeamAttributeSet* AuraAS = Cast<UTeamAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetActionAttribute()).AddUObject(
			this,
			&ATeamCombatCharacter::ActionAttributeChange
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FTeamGameplayTags::Get().Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ATeamCombatCharacter::HitReactTagChanged
		);
		
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
		OnActionChanged.Broadcast(0);
	}
}

void ATeamCombatCharacter::ActionAttributeChange(const FOnAttributeChangeData& Data)
{
	bTopOnAction = Data.NewValue >= 100;
	if (TeamAIController && TeamAIController->GetBlackboardComponent())
	{
		TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("TopOnAction"), bTopOnAction);
	}
	OnActionChanged.Broadcast(Data.NewValue);
}

void ATeamCombatCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (TeamAIController && TeamAIController->GetBlackboardComponent())
	{
		TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ATeamCombatCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TeamAIController = Cast<ATeamAIController>(NewController);
	TeamAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	TeamAIController->RunBehaviorTree(BehaviorTree);
	TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
	                                                           CharacterClass != ECharacterClass::Warrior);
	UCharacterClassInfo* CharacterClassInfo = UTeamAbilitySystemLibrary::GetCharacterClassInfo(this);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	TeamAIController->GetBlackboardComponent()->SetValueAsString(FName("SkillTag"), ClassDefaultInfo.SkillTag.ToString());
		
}

void ATeamCombatCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Cast<UTeamAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FTeamGameplayTags::Get().Debuff_Stun,
	                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ATeamCombatCharacter::StunTagChanged);


	InitializeDefaultAttributes();
	// OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void ATeamCombatCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (TeamAIController && TeamAIController->GetBlackboardComponent())
	{
		TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void ATeamCombatCharacter::InitializeDefaultAttributes() const
{
	UTeamAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ATeamCombatCharacter::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
	}
}

void ATeamCombatCharacter::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

void ATeamCombatCharacter::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// Do not change OutDestination
}

int32 ATeamCombatCharacter::GetPlayerLevel_Implementation()
{
	if (CharacterClass == ECharacterClass::Leader)
	{
		const ATeamPlayerState* TeamPlayerState = GetPlayerState<ATeamPlayerState>();
		check(TeamPlayerState);
		return TeamPlayerState->GetPlayerLevel();
	}
	return Level;
}

void ATeamCombatCharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (TeamAIController) TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	SpawnLoot();
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	// MulticastHandleDeath(DeathImpulse);
}

void ATeamCombatCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ATeamCombatCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

USkeletalMeshComponent* ATeamCombatCharacter::GetWeapon_Implementation()
{
	if (IsValid(Weapon))
	{
		return Weapon;
	}
	return GetMesh();
}

bool ATeamCombatCharacter::PlayHitEffect_Implementation(const FVector& HitLocation)
{
	if (BloodEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BloodEffect, HitLocation);
		return true;
	}
	return false;
}

FOnDeathSignature& ATeamCombatCharacter::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

FOnDamageSignature& ATeamCombatCharacter::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

TArray<FTaggedMontage> ATeamCombatCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedMontage ATeamCombatCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

FVector ATeamCombatCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FTeamGameplayTags& GameplayTags = FTeamGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}
