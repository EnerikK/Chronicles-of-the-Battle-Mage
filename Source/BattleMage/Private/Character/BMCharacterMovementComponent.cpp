// Hello :)


#include "Character/BMCharacterMovementComponent.h"
#include "Character/BMCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


UBmCharacterMovementComponent::UBmCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
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
}

uint8 UBmCharacterMovementComponent::FSavedMove_BattleMage::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if(Saved_bWantsToSprint)  Result |= FLAG_Custom_0;
	if(Saved_bWantsToSlide) Result |= FLAG_Custom_1;
	return  Result;

}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->bWantsToSprint;
	Saved_bWantsToSlide = CharacterMovement->bWantsToSlide;
}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->bWantsToSprint = Saved_bWantsToSprint;
	CharacterMovement->bWantsToSlide = Saved_bWantsToSlide;
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
	if(bWantsToSlide)
	{
		EnterSlide();
	}
	else
	{
		ExitSlide();
	}
	
}

void UBmCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case BMove_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp,Warning,TEXT("Invalid Mode"));
	
	}
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
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Client Tried to cheat"))
		}
	}
}

void UBmCharacterMovementComponent::OnSlideCooldownFinished()
{
	bWantsToSlide = true;
}

void UBmCharacterMovementComponent::EnterSlide()
{
	bWantsToSlide = true;
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom,BMove_Slide);
}
void UBmCharacterMovementComponent::ExitSlide()
{
	bWantsToSlide = false;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(),FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector,NewRotation,true,Hit);
	SetMovementMode(MOVE_Walking);
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
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->SlideMontage);
	SlideStartDelegate.Broadcast();
}

void UBmCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(Slide_MinSpeed, 2))
	{
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	// Surface Gravity
	Velocity += Slide_GravityForce * FVector::DownVector * deltaTime;

	// Strafe
	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	// Calc Velocity
	if(!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime, Slide_Friction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(Slide_MinSpeed, 2))
	{
		ExitSlide();
	}

	// Update Outgoing Velocity & Acceleration
	if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}
bool UBmCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f)* FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(Hit,Start,End,ProfileName);
}




