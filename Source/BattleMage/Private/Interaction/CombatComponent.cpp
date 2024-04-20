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

	if(Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshair(HitResult);
		HitTarget = HitResult.ImpactPoint;
		
		SetHudCrosshair(DeltaTime);
	}
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent,EquippedWeapon);
	DOREPLIFETIME(UCombatComponent,SecondaryWeapon);

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
	AWeapon* CurrentWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = CurrentWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EW_Equipped);
	AttachActorToRightHand(EquippedWeapon);

	SecondaryWeapon->SetWeaponState(EWeaponState::EW_EquippedSecondary);
	AttachActorToBackPack(SecondaryWeapon);
}

void UCombatComponent::TraceUnderCrosshair(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X/2.f,ViewportSize.Y/2.f);//Center of the viewport
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocation,CrossHairWorldPosition,CrossHairWorldDirection
		);
	if(bScreenToWorld)
	{
		FVector Start = CrossHairWorldPosition;

		if(Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrossHairWorldDirection * (DistanceToCharacter + 100.f);
			DrawDebugSphere(GetWorld(),Start,16.f,12,FColor::Red,false);
		}
		
		FVector End = Start + CrossHairWorldDirection * TRACE;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit) { TraceHitResult.ImpactPoint = End; }
		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UCombatInterface>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
	}
}

void UCombatComponent::SetHudCrosshair(float DeltaTime)
{
	if(Character == nullptr || Character->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<ABMPlayerController>(Character->Controller) : Controller; //Setting Controller variable and if
	//its set we just set the controller to itself and we dont have to to do the  cast again 
	
	if(Controller)
	{
		Hud = Hud == nullptr ? Cast<ABMHud>(Controller->GetHUD()) : Hud; //same
		if(Hud)
		{
			if(EquippedWeapon)
			{
				HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->CrosshairRight;
				HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairBottom;
				HUDPackage.CrosshairTop = EquippedWeapon->CrosshairTop;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
				HUDPackage.CrosshairTop = nullptr;
			}
			//Calculate crosshair spread
			//[0,600]->[0,1]
			FVector2d WalkSpeed(0.f,Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2d SpeedMultiplayer(0.f,1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;
			CrosshairVelocity = FMath::GetMappedRangeValueClamped(WalkSpeed,SpeedMultiplayer,Velocity.Size());
			
		}
	}
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
		if(Character)
		{
			Character->AttackIndexInCode = 0;
			UE_LOG(LogTemp,Warning,TEXT("Unoccupiead"));
		}
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


