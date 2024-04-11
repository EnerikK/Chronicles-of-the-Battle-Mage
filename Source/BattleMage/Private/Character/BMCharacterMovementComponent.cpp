// Hello :)


#include "Character/BMCharacterMovementComponent.h"
#include "Character/BMCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"


UBmCharacterMovementComponent::UBmCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

void UBmCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UBmCharacterMovementComponent,Proxy_bSlideStart,COND_SkipOwner);
}

void UBmCharacterMovementComponent::SprintPressed()
{
	bWantsToSprint = true;
}

void UBmCharacterMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
}

void UBmCharacterMovementComponent::CrouchPressed()
{
	 bWantsToCrouch = !bWantsToCrouch;
}

void UBmCharacterMovementComponent::SlidePressed()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if(CurrentTime - SlideStartTime >= Slide_CooldownDuration)
	{
		bWantsToSlide = true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer
		(TimerHandle_SlideCooldown,this,
		&UBmCharacterMovementComponent::OnSlideCooldownFinished,Slide_CooldownDuration - (CurrentTime - SlideStartTime));
	}
}

void UBmCharacterMovementComponent::SlideReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SlideCooldown);
	//bwantstoslide
	bWantsToSlide = false;
}

bool UBmCharacterMovementComponent::IsCustomMovementMode(EMovementModeBattleMage InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
void UBmCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	PlayerCharacter = Cast<ABMCharacter>(GetOwner());
	
}

bool UBmCharacterMovementComponent::FSavedMove_BattleMage::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                          ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_BattleMage* NewBattleMageMove = static_cast<FSavedMove_BattleMage*>(NewMove.Get());
	if(Saved_bWantsToSprint != NewBattleMageMove->Saved_bWantsToSprint)
	{
		return false;
	}
	if(Saved_bWantsToSlide != NewBattleMageMove->Saved_bWantsToSlide)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::Clear()
{
	FSavedMove_Character::Clear();
	
	Saved_bWantsToSprint = 0;
	Saved_bWantsToSlide = 0;
	Saved_bPressedBattleMageJump = 0;
	Saved_bHadAnimRootMotion = 0;
	Saved_bTranstionFinished = 0;
}

uint8 UBmCharacterMovementComponent::FSavedMove_BattleMage::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if(Saved_bWantsToSprint)  Result |= FLAG_Custom_0;
	if(Saved_bWantsToSlide) Result |= FLAG_Custom_1;
	if(Saved_bPressedBattleMageJump) Result |= FLAG_Custom_2;
	return  Result;

}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->bWantsToSprint;
	Saved_bWantsToSlide = CharacterMovement->bWantsToSlide;
	Saved_bPressedBattleMageJump = CharacterMovement->PlayerCharacter->bPressedBattleMageJump;
	Saved_bHadAnimRootMotion = CharacterMovement->bHadAnimRootMotion;
	Saved_bTranstionFinished = CharacterMovement->bTransitionFinished;
}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->bWantsToSprint = Saved_bWantsToSprint;
	CharacterMovement->bWantsToSlide = Saved_bWantsToSlide;
	CharacterMovement->PlayerCharacter->bPressedBattleMageJump = Saved_bPressedBattleMageJump;
	CharacterMovement->bHadAnimRootMotion = Saved_bHadAnimRootMotion;
	CharacterMovement->bTransitionFinished = Saved_bTranstionFinished;
}

UBmCharacterMovementComponent::FNetworkPredictionData_Client_BattleMage::FNetworkPredictionData_Client_BattleMage(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
	
}
FSavedMovePtr UBmCharacterMovementComponent::FNetworkPredictionData_Client_BattleMage::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_BattleMage());
}
FNetworkPredictionData_Client* UBmCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)
	if(ClientPredictionData == nullptr)
	{
		UBmCharacterMovementComponent* MutableThis = const_cast<UBmCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_BattleMage(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

bool UBmCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(BMove_Slide);
}

bool UBmCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UBmCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToSprint = (Flags & FSavedMove_BattleMage::FLAG_Custom_0) != 0;
	//bwants to slide
	bWantsToSlide = (Flags & FSavedMove_BattleMage::FLAG_Custom_1) != 0;
}

void UBmCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
	
}

void UBmCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
}

void UBmCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
	/*Slide*/
	
	bool bAuthProxy = CharacterOwner->HasAuthority() && !CharacterOwner->IsLocallyControlled();
	if(bWantsToSlide && CanSlide())
	{
		if(!bAuthProxy || GetWorld()->GetTimeSeconds() - SlideStartTime > AuthSlideCooldownDuration)
		{
			PerformSlide();
			bWantsToSlide = false;
			Proxy_bSlideStart = !Proxy_bSlideStart;
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Client Tried to cheat"))
		}
	}
	/*try Mantle*/
	if(PlayerCharacter->bPressedBattleMageJump)
	{
		if(TryMantle())
		{
			PlayerCharacter->StopJumping();
		}
		else
		{
			PlayerCharacter->bPressedBattleMageJump = false;
			CharacterOwner->bPressedJump = true;
			CharacterOwner->CheckJumpInput(DeltaSeconds);
		}
	}
}

void UBmCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
	
}

void UBmCharacterMovementComponent::OnRep_SlideStart()
{
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->GetSlideMontage());
	SlideStartDelegate.Broadcast();
}
void UBmCharacterMovementComponent::OnSlideCooldownFinished()
{
	bWantsToSlide = true;

}
bool UBmCharacterMovementComponent::CanSlide() const
{
	return IsWalking() && !IsCrouching();
}

void UBmCharacterMovementComponent::PerformSlide()
{
	SlideStartTime = GetWorld()->GetTimeSeconds();
	FVector SlideDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration).GetSafeNormal2D();
	Velocity = Slide_EnterImpulse * SlideDirection;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(SlideDirection,FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector,NewRotation,false,Hit);
	SetMovementMode(MOVE_Walking);
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->GetSlideMontage());
	SlideStartDelegate.Broadcast();
}
/*Mantle*/
void UBmCharacterMovementComponent::OnRep_ShortMantle()
{
	
}
void UBmCharacterMovementComponent::OnRep_TallMantle()
{
	
}
bool UBmCharacterMovementComponent::TryMantle() const
{
	return false;
}
FVector UBmCharacterMovementComponent::GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit,
	bool bTallMantle)
{
	return FVector::ZeroVector;
}

#pragma region Helpers
bool UBmCharacterMovementComponent::IsServer() const
{
	return CharacterOwner->HasAuthority();
}

float UBmCharacterMovementComponent::CapR() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UBmCharacterMovementComponent::CapHH() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}
#pragma endregion 

