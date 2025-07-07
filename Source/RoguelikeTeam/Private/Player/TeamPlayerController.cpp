// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TeamGameplayTags.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "Formation/FormationActorComponent.h"
#include "GameFramework/Pawn.h"
#include "Input/TeamInputComponent.h"
#include "Player/TeamPlayerState.h"
#include "Windows/AllowWindowsPlatformTypes.h"

class UFormationActorComponent;
class ATeamPlayerState;

ATeamPlayerController::ATeamPlayerController()
{
}

void ATeamPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FaceToMouseCursor();
}

void ATeamPlayerController::BroadcastTeamMoveEvent(FVector NewLocation)
{
}

void ATeamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UTeamInputComponent* TeamInputComponent = CastChecked<UTeamInputComponent>(InputComponent);
	TeamInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeamPlayerController::Move);
	TeamInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATeamPlayerController::Rotate);
	// AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	// AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	// AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATeamPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) 
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor =  true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

UTeamAbilitySystemComponent* ATeamPlayerController::GetASC()
{
	if (TeamAbilitySystemComponent == nullptr)
	{
		TeamAbilitySystemComponent = Cast<UTeamAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TeamAbilitySystemComponent;
}

void ATeamPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FTeamGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

		ATeamPlayerState* PS = GetPlayerState<ATeamPlayerState>();
		check(PS);
		if(UFormationActorComponent* Formation = PS->GetFormationActorComponent())
		{
			FVector ActorLocation = ControlledPawn -> GetActorLocation();
			Formation->UpdateAllMemberPositions(ActorLocation);
		}
	}
}


void ATeamPlayerController::Rotate(const FInputActionValue& InputActionValue)
{
	bool isRight = InputActionValue.Get<bool>();

	ATeamPlayerState* PS = GetPlayerState<ATeamPlayerState>();
	check(PS);
	if(UFormationActorComponent* Formation = PS->GetFormationActorComponent())
	{
		APawn* ControlledPawn = GetPawn<APawn>();
		FVector ActorLocation = ControlledPawn -> GetActorLocation();
		Formation -> RotateFormation(isRight? .5f : -.5f);
		Formation->UpdateAllMemberPositions(ActorLocation);
	}
	// if (UAbilitySystemComponent* ASC = GetASC())
	// {
	// 	FGameplayEventData EventData;
	// 	EventData.EventMagnitude = isRight ? 1.f : -1.f;
 //        
	// 	// ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Input.Rotate"), &EventData);
	// }
}


void ATeamPlayerController::Focus(const FInputActionValue& InputActionValue)
{
	FaceToMouseCursor();
}

void ATeamPlayerController::FaceToMouseCursor()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		// 获取鼠标位置
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
        
		if (HitResult.bBlockingHit)
		{
			// 计算朝向方向
			FVector Direction = (HitResult.ImpactPoint - ControlledPawn->GetActorLocation()).GetSafeNormal();
			FRotator TargetRotation = Direction.Rotation();
            
			// 保持Z轴不变，只旋转Yaw
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;
            
			// 设置角色朝向
			ControlledPawn->SetActorRotation(TargetRotation);
		}
	}
}
