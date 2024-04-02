//Hello :)

#include "Player/BMPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"


ABMPlayerController::ABMPlayerController()
{
	bReplicates = true;
}

void ABMPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
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
