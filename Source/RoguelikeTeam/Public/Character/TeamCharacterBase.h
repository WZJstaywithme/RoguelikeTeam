// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "TeamCharacterBase.generated.h"

class UAttributeSet;
class UMotionWarpingComponent;
class UAbilitySystemComponent;

UCLASS(Abstract)
class ROGUELIKETEAM_API ATeamCharacterBase : public ACharacter, public ICombatInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATeamCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/** end Combat Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	/** end Combat Interface */

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	virtual void InitAbilityActorInfo();

	virtual void InitializeDefaultAttributes() const;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Combat")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
};
