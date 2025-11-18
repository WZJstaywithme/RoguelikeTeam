// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeamCombatCharacter.h"

#include "AbilitySystemComponent.h"
#include "TeamGameplayTags.h"
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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	if (IsValid(Weapon))
	{
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
		Weapon->MarkRenderStateDirty();
	}
	
	BaseWalkSpeed = 250.f;
}

void ATeamCombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UTeamAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	// if (UTeamUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	// {
	// 	AuraUserWidget->SetWidgetController(this);
	// }

	if (const UTeamAttributeSet* AuraAS = Cast<UTeamAttributeSet>(AttributeSet))
	{
		// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
		// 	[this](const FOnAttributeChangeData& Data)
		// 	{
		// 		OnHealthChanged.Broadcast(Data.NewValue);
		// 	}
		// );
		// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
		// 	[this](const FOnAttributeChangeData& Data)
		// 	{
		// 		OnMaxHealthChanged.Broadcast(Data.NewValue);
		// 	}
		// );

		AbilitySystemComponent->RegisterGameplayTagEvent(FTeamGameplayTags::Get().Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ATeamCombatCharacter::HitReactTagChanged
		);

		// OnHealthChanged.Broadcast(AuraAS->GetHealth());
		// OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
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
	TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void ATeamCombatCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Cast<UTeamAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FTeamGameplayTags::Get().Debuff_Stun,
	                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ATeamCombatCharacter::StunTagChanged);


	InitializeDefaultAttributes();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
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
	return Level;
}

void ATeamCombatCharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (TeamAIController) TeamAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	SpawnLoot();
	Super::Die(DeathImpulse);
}

void ATeamCombatCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ATeamCombatCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

