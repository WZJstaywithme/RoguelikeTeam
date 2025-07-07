// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TeamAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "TeamPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamMoveEvent, FVector, NewLocation);

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class ROGUELIKETEAM_API ATeamPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATeamPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	UTeamAbilitySystemComponent* GetASC();

	UPROPERTY(BlueprintAssignable, Category = "Team")
	FOnTeamMoveEvent OnTeamMoveEvent;

	// 广播移动事件给所有队友
	UFUNCTION(BlueprintCallable, Category = "Team")
	void BroadcastTeamMoveEvent(FVector NewLocation);

protected:
	// virtual void BeginPlay() override;
	virtual void SetupInputComponent()  override;
	void BeginPlay();

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(EditAnywhere, Category="Ability")
	TObjectPtr<UTeamAbilitySystemComponent> TeamAbilitySystemComponent;

	void Move(const FInputActionValue& InputActionValue);
	void Rotate(const FInputActionValue& InputActionValue);
	void Focus(const FInputActionValue& InputActionValue);
	void FaceToMouseCursor();
};
