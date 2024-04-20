//Hello :)


#include "Character/BMCharacter.h"
#include "AbilitySystemComponent.h"
#include "AnalyticsEventAttribute.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/BMPlayerController.h"
#include "Player/BMPlayerState.h"
#include "Weapon/Weapon.h"

ABMCharacter::ABMCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(
		  ACharacter::CharacterMovementComponentName))
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	BMCharacterMovementComponent = Cast<UBmCharacterMovementComponent>(GetCharacterMovement());
	BMCharacterMovementComponent->SetIsReplicated(true);

	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
	Combat->SetIsReplicated(true);

	TurningInPlace = ETurnInPlace::ETurnIP_NotTurning;
}

void ABMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotateInPlace(DeltaSeconds);
}
void ABMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SlideStartDelegate.Broadcast();
}
void ABMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ABMCharacter::SwapWeaponTimerFinished()
{
}

void ABMCharacter::RotateInPlace(float DeltaTime)
{
	if(Combat && Combat->EquippedWeapon)
	{
		/*GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;*/
	}
	if(PlayerController)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurnInPlace::ETurnIP_NotTurning;
		return;
		
	}
	AimOffset(DeltaTime);
}
void ABMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABMCharacter,OverlappingWeapon,COND_OwnerOnly);

}

void ABMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void ABMCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
	}
	
}
/*Weapons*/
void ABMCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}
	if(LastWeapon)
	{
		LastWeapon->ShowPickUpWidget(false);
	}
}
void ABMCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(false);
	}
	OverlappingWeapon = Weapon;
	if(IsLocallyControlled())
	{
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}
}

void ABMCharacter::EquipButtonPressed()
{
	if(Combat)
	{
		if(Combat->CombatState == ECombatState::ECState_Unoccupied) ServerEquipButtonPressed();
		if(Combat->bShouldSwapWeapon() && !HasAuthority() && Combat->CombatState == ECombatState::ECState_Unoccupied && OverlappingWeapon == nullptr)
		{
			PlaySwapMontage();
			Combat->CombatState = ECombatState::ECState_SwapWeapons;
			bFinishedSwapping = false;
		}
	}
}
AWeapon* ABMCharacter::GetEquippedWeapon()
{
	if(Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}
bool ABMCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

void ABMCharacter::PlaySwapMontage()
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && WeaponSwapMontage)
	{
		AnimInstance->Montage_Play(WeaponSwapMontage);
	}
}

void ABMCharacter::SetStateInCode(ECombatState NewState)
{
	if(NewState != CurrentState)
	{
		CurrentState = NewState;
	}
}

bool ABMCharacter::IsStateEqualToAnyInCode(TArray<ECombatState> StatesToCheck)
{
	if(StatesToCheck.Contains(CurrentState))
	{
		return true;
	}
	return false;
}

bool ABMCharacter::PerformLightAttackInCode(int32 CurrentAttackIndex)
{
	AttackIndexInCode = CurrentAttackIndex;
	if(IsWeaponEquipped() && Combat->CombatState == ECombatState::ECState_Unoccupied)
	{
		UAnimMontage* LightAttackMontage = Combat->EquippedWeapon->AttackMontages[AttackIndexInCode];
		if(LightAttackMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance)
			{
				AnimInstance->Montage_Play(LightAttackMontage,0.7);
			}
			//PlayAnimMontage(LightAttackMontage);
			SetStateInCode(ECombatState::ECState_Attack);
			AttackIndexInCode++;
			if(AttackIndexInCode >=  Combat->EquippedWeapon->AttackMontages.Num())
			{
				AttackIndexInCode = 0;
				return true;
			}
			return false;
		}
	}
	return false;
}

bool ABMCharacter::PerformHeavyAttackInCode(int32 CurrentAttackIndex)
{
	HeavyAttackIndexInCode = CurrentAttackIndex;
	if(IsWeaponEquipped() && Combat->CombatState == ECombatState::ECState_Unoccupied)
	{
		UAnimMontage* HeavyAttackMontage = Combat->EquippedWeapon->HeavyAttackMontages[HeavyAttackIndexInCode];
		if(HeavyAttackMontage)
		{
			SetStateInCode(ECombatState::ECState_Attack);
			PlayAnimMontage(HeavyAttackMontage,0.9);
			HeavyAttackIndexInCode++;
			if(HeavyAttackIndexInCode >=  Combat->EquippedWeapon->HeavyAttackMontages.Num())
			{
				HeavyAttackIndexInCode = 0;
				return true;
			}
			return false;
		}
	}
	return false;
}

void ABMCharacter::AttackEvent()
{
	if(Combat->CombatState != ECombatState::ECState_Attack)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle,this,&ABMCharacter::TimerBeforePressingAgain,0.8f);
		PerformLightAttackInCode(AttackIndexInCode);
	}
}

void ABMCharacter::HeavyAttackEvent()
{
	if(Combat->CombatState != ECombatState::ECState_Attack)
	{
		PerformHeavyAttackInCode(HeavyAttackIndexInCode);
	}
}

void ABMCharacter::ResetValues()
{
	SetStateInCode(ECombatState::ECState_Unoccupied);
	AttackIndexInCode = 0;
	HeavyAttackIndexInCode = 0;
	bSaveLightAttack = false;
	bSaveHeavyAttack = false;
}

void ABMCharacter::SaveLightAttack()
{
	if(bSaveLightAttack)
	{
		bSaveLightAttack = false;
		if(Combat->CombatState == ECombatState::ECState_Attack)
		{
			SetStateInCode(ECombatState::ECState_Unoccupied);
			AttackEvent();
		}
		AttackEvent();
	}
}

void ABMCharacter::SaveHeavyAttack()
{
	if(!bSaveHeavyAttack)
	{
		bSaveHeavyAttack = false;
		if(Combat->CombatState == ECombatState::ECState_Attack)
		{
			SetStateInCode(ECombatState::ECState_Unoccupied);
			HeavyAttackEvent();
		}
		
	}
}

void ABMCharacter::TimerBeforePressingAgain()
{
	
}

void ABMCharacter::ServerEquipButtonPressed_Implementation()
{
	if(Combat)
	{
		if(OverlappingWeapon)
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else if (Combat->bShouldSwapWeapon() && Combat->CombatState != ECombatState::ECState_Attack)
		{
			Combat->SwapWeapon();
		}
	}
}
/*End Weapons*/
/*Character*/
bool ABMCharacter::IsCrouching()
{
	if(GetBMCharacterComponent()->bWantsToCrouch)
	{
		return true;
	}
	return false;
}
bool ABMCharacter::IsSliding()
{
	if(GetBMCharacterComponent()->IsSliding())
	{
		return true;
	}
	return false;
}
void ABMCharacter::Jump()
{
	bPressedBattleMageJump = true;
	bPressedJump = false;
	Super::Jump();
}
void ABMCharacter::StopJumping()
{
	Super::StopJumping();
	bPressedBattleMageJump = false;
}
void ABMCharacter::InitAbilityActorInfo()
{
	ABMPlayerState* BattleMagePlayerState = GetPlayerState<ABMPlayerState>();
	check(BattleMagePlayerState)

	BattleMagePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BattleMagePlayerState,this);
	Cast<UBMAbilitySystemComponent>(BattleMagePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = BattleMagePlayerState->GetAbilitySystemComponent();AttributeSet = BattleMagePlayerState->GetAttributeSet();
	AttributeSet = BattleMagePlayerState->GetAttributeSet();

	OnAscRegistered.Broadcast(AbilitySystemComponent);
	if(ABMPlayerController* BattleMagePlayerController = Cast<ABMPlayerController>(GetController()))
	{
		if(ABMHud* BattleMageHud = Cast<ABMHud>(BattleMagePlayerController->GetHUD()))
		{
			BattleMageHud->InitOverlay(BattleMagePlayerController,BattleMagePlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
void ABMCharacter::AimOffset(float DeltaTime)
{
	//There's a Bug on the Yaw when you use it on Multiplayer Fix when you can if  you can future me
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurnInPlace::ETurnIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = false;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurnInPlace::ETurnIP_NotTurning;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		//found out that when i try to pitch downwards thepitch goes from 360 to 270 it should be 0 to -90 so we MAP pitch from [270,360) to [-90,0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		//Apply Correction
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}
void ABMCharacter::TurnInPlace(float DeltaTime)
{
	//UE_LOG(LogTemp,Warning,TEXT("AO_YAW : %f"),AO_Yaw);
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurnInPlace::ETurnIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurnInPlace::ETurnIP_Left;
	}
	if(TurningInPlace != ETurnInPlace::ETurnIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw,0.f,DeltaTime,4.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurnInPlace::ETurnIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			
		}
	}
}
/*EndCharacter*/
