// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamUserWidget.h"
#include "Character/TeamCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TeamCombatCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ATeamAIController;
/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamCombatCharacter : public ATeamCharacterBase,public ICombatInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ATeamCombatCharacter();
	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** end Highlight Interface */

	/** Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual bool PlayHitEffect_Implementation(const FVector& HitLocation) override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;   
	/** end Combat Interface */

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnActionChanged;


	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bTopOnAction = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "UI")
	TArray<TSubclassOf<UTeamUserWidget>> BarWidgets;

	void SetLevel(int32 InLevel) { Level = InLevel; }

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;

protected:
	virtual void BeginPlay() override;
	void ActionAttributeChange(const FOnAttributeChangeData& Data);
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> BarContainer;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ATeamAIController> TeamAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;
};
