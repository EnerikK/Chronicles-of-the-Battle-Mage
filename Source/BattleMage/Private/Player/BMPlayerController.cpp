//Hello :)

#include "Player/BMPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BMGameplayTags.h"
#include "Character/BMCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/EnemyInterface.h"


ABMPlayerController::ABMPlayerController()
{
	bReplicates = true;
}

void ABMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CursorTrace();
}

void ABMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(BMContest);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(BMContest,0);
	}
}

void ABMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(
		MoveAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Move);
	EnhancedInputComponent->BindAction(
		LookAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Look);
	EnhancedInputComponent->BindAction(
		JumpAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Jump);
	EnhancedInputComponent->BindAction(
		EquipAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Equip);
	
	
}

void ABMPlayerController::CursorTrace()
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBattleMageGameplayTags::Get().Player_Block_CursorTrace))
	{
		if(LastActor) LastActor->UnHighlight();
		if(ThisActor) ThisActor->Highlight();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
		
	}
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	if(LastActor != ThisActor)
	{
		if(LastActor) LastActor->UnHighlight();
		if(ThisActor) ThisActor->Highlight();
	}
	
	
}

UBMAbilitySystemComponent* ABMPlayerController::GetASC()
{
	if(AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UBMAbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void ABMPlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn<APawn>();

	if(ControlledPawn != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0,ControlledPawn->GetControlRotation().Yaw,0);

		if(MoveValue.Y != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			ControlledPawn->AddMovementInput(Direction,MoveValue.Y);
		}
		if(MoveValue.X != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			ControlledPawn->AddMovementInput(Direction,MoveValue.X);
		}
	}
	
}

void ABMPlayerController::Look(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn<APawn>();

	if(ControlledPawn->Controller != nullptr)
	{
		const FVector2d LookValue = Value.Get<FVector2d>();
		
		if(LookValue.X != 0.f)
		{
			ControlledPawn->AddControllerYawInput(LookValue.X);
		}
		
		if(LookValue.Y != 0.f)
		{
			ControlledPawn->AddControllerPitchInput(LookValue.Y);
		}
	}
}

void ABMPlayerController::Jump(const FInputActionValue& Value)
{
	if(ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}
}

void ABMPlayerController::Equip(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->EquipButtonPressed();
	}
}