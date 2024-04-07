// Hello :)


#include "Character/BMCharacterMovementComponent.h"

#include "Character/BMCharacter.h"
#include "GameFramework/Character.h"



UBmCharacterMovementComponent::UBmCharacterMovementComponent()
{
}

void UBmCharacterMovementComponent::SprintPressed()
{
	bWantsToSprint = true;
}
void UBmCharacterMovementComponent::SprintReleased()
{
	bWantsToSprint = false;
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

	UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->bWantsToSprint;
}

void UBmCharacterMovementComponent::FSavedMove_BattleMage::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	UBmCharacterMovementComponent* CharacterMovement = Cast<UBmCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->bWantsToSprint = Saved_bWantsToSprint;
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
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Character(*this);
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
	
}


