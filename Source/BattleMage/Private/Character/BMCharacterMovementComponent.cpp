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
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::Clear()
{
	FSavedMove_Character::Clear();
	
	Saved_bWantsToSprint = 0;
}

uint8 UBmCharacterMovementComponent::FSavedMove_BattleMage::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if(Saved_bWantsToSprint)  Result |= FLAG_Custom_0;
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
void UBmCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
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
	if(MovementMode == BMove_Slide)
	{
		if(bWantsToSlide)
		{
			
		}
	}
	bPrev_WantsToCrouch = bWantsToCrouch;
}

void UBmCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if(MovementMode == MOVE_Walking && !bWantsToCrouch && bPrev_WantsToCrouch)
	{
		FHitResult PotentialSlideSurface;
		if(Velocity.SizeSquared() > pow(Slide_MinSpeed,2) && GetSlideSurface(PotentialSlideSurface))
		{
			EnterSlide();
		}
	}
	if(IsCustomMovementMode(BMove_Slide) && !bWantsToCrouch)
	{
		ExitSlide();
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UBmCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case BMove_Slide:
		PhysSlide(deltaTime,Iterations);
		break;
	default:
		UE_LOG(LogTemp,Error,TEXT("Invalid MovementMode"))
	}
}

bool UBmCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(BMove_Slide);
}

bool UBmCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UBmCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom,BMove_Slide);
}

void UBmCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;
	
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(),FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector,NewRotation,true,Hit);
	SetMovementMode(MOVE_Walking);
}

void UBmCharacterMovementComponent::PhysSlide(float DeltaTime, int32 It)
{
	if(DeltaTime < MIN_TICK_TIME)
	{
		return;
	}
	//RestorePreAdditiveRootMotionVelocity(); // use this if animation has root motion 
	FHitResult SurfaceHit;
	if(!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(Slide_MinSpeed , 2))
	{
		ExitSlide();
		StartNewPhysics(DeltaTime,It);
		return;
	}

	/*Surface Gravity*/
	Velocity += Slide_GravityForce * FVector::DownVector * DeltaTime;
	
	/*Strafe*/
	if(FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(),UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	/*Calculate Velocity*/
	if(!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(DeltaTime,Slide_Friction,true,GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(DeltaTime);

	/*Perform Move*/
	It++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * DeltaTime;
	FVector VelPlanerDir = FVector::VectorPlaneProject(Velocity,SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlanerDir,SurfaceHit.Normal).ToQuat();
	SafeMoveUpdatedComponent(Adjusted,NewRotation,true,Hit);

	if(Hit.Time < 1.f)
	{
		HandleImpact(Hit,DeltaTime,Adjusted);
		SlideAlongSurface(Adjusted,(1.f - Hit.Time),Hit.Normal,Hit,true);
	}
	FHitResult NewSurfaceHit;
	if(!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(Slide_MinSpeed , 2))
	{
		ExitSlide();
	}

	/*Update outgoing velocity*/
	if(!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime; // v = dx / dt
	}
	
}

bool UBmCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = (Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f) * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	
	return GetWorld()->LineTraceSingleByProfile(Hit,Start,End,ProfileName);
}


