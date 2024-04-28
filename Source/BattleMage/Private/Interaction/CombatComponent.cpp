// Hello :)


#include "Interaction/CombatComponent.h"

#include "Character/BMCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/BMPlayerController.h"
#include "Types/CombatState.h"
#include "Weapon/Weapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	BaseWalkSpeed = 600.f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	
}
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCombatComponent,EquippedWeapon);
	DOREPLIFETIME(UCombatComponent,SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent,CombatState);

}
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;
	if(CombatState != ECombatState::ECState_Unoccupied) return;
	
	if(EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
	{
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else
	{
		EquipPrimaryWeapon(WeaponToEquip);
	}
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;

}

bool UCombatComponent::bShouldSwapWeapon()
{
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);
}

void UCombatComponent::SwapWeapon()
{
	if(CombatState != ECombatState::ECState_Unoccupied || Character == nullptr || !Character->HasAuthority()) return;
	
	Character->PlaySwapMontage();
	CombatState = ECombatState::ECState_SwapWeapons;
	Character->bFinishedSwapping = false;
}

void UCombatComponent::FinishSwap()
{
	if(Character && Character->HasAuthority())
	{
		CombatState = ECombatState::ECState_Unoccupied;
	}
	if(Character) Character->bFinishedSwapping = true;
}

void UCombatComponent::FinishSwapWeapon()
{
	if (Character == nullptr || !Character->HasAuthority()) return;
	
	AWeapon* CurrentWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = CurrentWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EW_Equipped);
	AttachActorToRightHand(EquippedWeapon);

	SecondaryWeapon->SetWeaponState(EWeaponState::EW_EquippedSecondary);
	AttachActorToBackPack(SecondaryWeapon);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EW_Equipped);
		AttachActorToRightHand(EquippedWeapon);
		
	}
}
void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
{
	if(Character == nullptr || ActorToAttach == nullptr || Character->GetMesh() == nullptr) return;
	
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("Hand_L_Socket"));
	if(HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach,Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToBackPack(AActor* ActorToAttach)
{
	if(Character == nullptr || Character->GetMesh() == nullptr|| ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* BackpackSocket = Character->GetMesh()->GetSocketByName(FName("BackSocket"));
	if(BackpackSocket)
	{
		BackpackSocket->AttachActor(ActorToAttach,Character->GetMesh());
	}
}
void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if(Character == nullptr || ActorToAttach == nullptr || Character->GetMesh() == nullptr || EquippedWeapon == nullptr) return;
	/*bool bUseRocketSocket = EquippedWeapon->GetWeaponType() == EWeaponType::EWT_RocketLauncher;
	FName SocketName = bUseRocketSocket ? FName("Hand_R_SocketRocket") : FName("Hand_R_Socket");*/ // Using a specific socket for a specific gun if need 
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("Hand_R_Socket"));
	if(HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach,Character->GetMesh());
	}
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	if(SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EW_EquippedSecondary);
		AttachActorToBackPack(SecondaryWeapon);
	}
}

void UCombatComponent::DropEquippedWeapon()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if(WeaponToEquip == nullptr) return;
	DropEquippedWeapon();
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EW_Equipped);
	
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetOwner(Character);
	
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if(WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EW_EquippedSecondary);
	AttachActorToBackPack(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECState_Reloading:
		if(Character && !Character->IsLocallyControlled())
		break;
	case ECombatState::ECState_Unoccupied:
		/*if(Character)
		{
			Character->AttackIndexInCode = 0;
			UE_LOG(LogTemp,Warning,TEXT("Unoccupiead"));
		}*/
			break;
	case ECombatState::ECState_ThrowGrenade:
		if(Character && !Character->IsLocallyControlled())
		{
			AttachActorToLeftHand(EquippedWeapon);
		}
		break;
	case ECombatState::ECState_SwapWeapons:
		if(Character && !Character->IsLocallyControlled())
		{
			Character->PlaySwapMontage();
		}
		break;
	}
}


