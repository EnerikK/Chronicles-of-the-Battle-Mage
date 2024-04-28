//Hello :)

#include "Player/BMPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BMGameplayTags.h"
#include "Character/BMCharacter.h"
#include "Character/BMCharacterMovementComponent.h"
#include "AbilitySystem/Abilities/BMGameplayAbility.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Input/BMInputComponent.h"
#include "Interaction/CombatComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Net/UnrealNetwork.h"


ABMPlayerController::ABMPlayerController()
{
	bReplicates = true;
}

void ABMPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABMPlayerController,bDisableGameplay);
}

void ABMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CursorTrace();
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

void ABMPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBattleMageGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	
	if(GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}
void ABMPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBattleMageGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if(!InputTag.MatchesTagExact(FBattleMageGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	
	if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void ABMPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBattleMageGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if(!InputTag.MatchesTagExact(FBattleMageGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())GetASC()->AbilityInputTagHeld(InputTag);
		return;
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
void ABMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBMInputComponent* BattleMageInputComponent = CastChecked<UBMInputComponent>(InputComponent);

	BattleMageInputComponent->BindAbilityActions(
		InputConfig, this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
	
	BattleMageInputComponent->BindAction(
		MoveAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Move);
	BattleMageInputComponent->BindAction(
		LookAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Look);
	BattleMageInputComponent->BindAction(
		JumpAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Jump);
	BattleMageInputComponent->BindAction(
		EquipAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Equip);
	BattleMageInputComponent->BindAction(
		ShiftPressed,ETriggerEvent::Started,this,&ABMPlayerController::StartSprint);
	BattleMageInputComponent->BindAction(
		ShiftPressed,ETriggerEvent::Completed,this,&ABMPlayerController::StopSprint);
	BattleMageInputComponent->BindAction(
		CrouchAction,ETriggerEvent::Triggered,this,&ABMPlayerController::Crouch);
	BattleMageInputComponent->BindAction(
		SlideAction,ETriggerEvent::Started,this,&ABMPlayerController::Slide);
	BattleMageInputComponent->BindAction(
		SlideAction,ETriggerEvent::Completed,this,&ABMPlayerController::SlideReleased);
	BattleMageInputComponent->BindAction(
		AttackAction,ETriggerEvent::Started,this,&ABMPlayerController::Attack);
	BattleMageInputComponent->BindAction(
		HeavyAttackAction,ETriggerEvent::Started,this,&ABMPlayerController::HeavyAttack);
}
void ABMPlayerController::Move(const FInputActionValue& Value)
{
	if(bDisableGameplay) return;
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
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->Jump();
	}
}

void ABMPlayerController::Equip(const FInputActionValue& Value)
{
	if(bDisableGameplay) return;
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->EquipButtonPressed();
	}
}

void ABMPlayerController::StartSprint(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->GetBMCharacterComponent()->SprintPressed();
	}
}

void ABMPlayerController::StopSprint(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->GetBMCharacterComponent()->SprintReleased();
	}
}
void ABMPlayerController::Crouch(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->GetBMCharacterComponent()->CrouchPressed();
	}
}

void ABMPlayerController::Slide(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->GetBMCharacterComponent()->SlidePressed();
	}
}

void ABMPlayerController::SlideReleased(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
		ControlledCharacter->GetBMCharacterComponent()->SlideReleased();
	}
}

void ABMPlayerController::Attack(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
	}
}

void ABMPlayerController::HeavyAttack(const FInputActionValue& Value)
{
	if(ABMCharacter* ControlledCharacter = Cast<ABMCharacter>(GetCharacter()))
	{
	}
}



