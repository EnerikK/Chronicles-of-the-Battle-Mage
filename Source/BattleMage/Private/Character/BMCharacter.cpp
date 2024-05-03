//Hello :)


#include "Character/BMCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BMGameplayTags.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/BMMotionWarping.h"
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
	PrimaryActorTick.bCanEverTick = true;
	
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
	
	BMMotionWarping = CreateDefaultSubobject<UBMMotionWarping>("BMMotionWarping");
	BMMotionWarping->SetIsReplicated(true);
	
	TurningInPlace = ETurnInPlace::ETurnIP_NotTurning;
}

void ABMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotateInPlace(DeltaSeconds);
	
}

FVector ABMCharacter::GetHitTarget() const
{
	if(Combat == nullptr) return FVector();
	return Combat->HitTarget;
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

void ABMCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
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
	if(BMMotionWarping)
	{
		BMMotionWarping->Character = this;
	}
	
}

int32 ABMCharacter::GetPlayerLevel_Implementation()
{
	const ABMPlayerState* BattleMagePlayerState = GetPlayerState<ABMPlayerState>();
	check(BattleMagePlayerState);
	return BattleMagePlayerState->GetPlayerLevel();
	
}

void ABMCharacter::WeaponCollision_Implementation(AWeapon* CurrentWeapon,float Radius, float End)
{
	if(IsWeaponEquipped())
	{
		CurrentWeapon = Combat->EquippedWeapon;
		FVector StartSphere = Combat->EquippedWeapon->GetWeaponMesh()->GetSocketLocation("Start");
		FVector EndSphere = Combat->EquippedWeapon->GetWeaponMesh()->GetSocketLocation("End");
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());
		TArray<FHitResult> HitArray;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
		objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());

		const bool Hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),StartSphere,EndSphere,Radius,
			objectTypesArray,false,
			ActorsToIgnore,EDrawDebugTrace::Persistent,HitArray,true,FLinearColor::Gray,
			FLinearColor::Blue,5.f);
		
		if(Hit)
		{
			for(const FHitResult HitResult : HitArray)
			{
				AActor* HitActor = HitResult.GetActor();
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,FString::Printf(TEXT("Hit %s"),*HitActor->GetName()));
				
				if(HasAuthority())
				{
					if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
					{
						TargetASC->ApplyGameplayEffectSpecToSelf(*CurrentWeapon->DamageEffectSpecHandle.Data.Get());
					}
		
				}
			}
		}
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && GetWeaponSwapMontage())
	{
		AnimInstance->Montage_Play(GetWeaponSwapMontage());
	}
}

ECombatState ABMCharacter::GetCombatState() const
{
	if(Combat == nullptr) return ECombatState::ECState_MAX;
	return Combat->CombatState;
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
void ABMCharacter::PerformLightAttackInCode(int32 CurrentAttackIndex)
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
				AnimInstance->Montage_Play(LightAttackMontage);
			}
			//PlayAnimMontage(LightAttackMontage);
			SetStateInCode(ECombatState::ECState_Attack);
			AttackIndexInCode++;
			if(AttackIndexInCode >=  Combat->EquippedWeapon->AttackMontages.Num())
			{
				AttackIndexInCode = 0;
			}
		}
	}
}
bool ABMCharacter::PerformHeavyAttackInCode(int32 CurrentAttackIndex)
{
	HeavyAttackIndexInCode = CurrentAttackIndex;
	if(IsWeaponEquipped() && Combat->CombatState != ECombatState::ECState_HeavyAttack)
	{
		UAnimMontage* HeavyAttackMontage = Combat->EquippedWeapon->HeavyAttackMontages[HeavyAttackIndexInCode];
		if(HeavyAttackMontage)
		{
			SetStateInCode(ECombatState::ECState_HeavyAttack);
			PlayAnimMontage(HeavyAttackMontage);
			HeavyAttackIndexInCode++;
			if(HeavyAttackIndexInCode >=  Combat->EquippedWeapon->HeavyAttackMontages.Num())
			{
				HeavyAttackIndexInCode = 0;
				return true;
			}
			return true;
		}
	}
	return false;
}
void ABMCharacter::AttackEvent()
{
	if(Combat->CombatState != ECombatState::ECState_Attack)
	{
		PerformLightAttackInCode(AttackIndexInCode);
	}
}

void ABMCharacter::HeavyAttackEvent()
{
	if(Combat->CombatState != ECombatState::ECState_HeavyAttack)
	{
		PerformHeavyAttackInCode(HeavyAttackIndexInCode);
	}
}
void ABMCharacter::SaveLightAttack()
{
	if(bSaveLightAttack)
	{
		bSaveLightAttack = false;
		if(Combat->CombatState == ECombatState::ECState_Attack)
		{
			AttackEvent();
			SetStateInCode(ECombatState::ECState_Unoccupied);
		}
		AttackEvent();
	}
}

void ABMCharacter::SaveHeavyAttack()
{
	if(bSaveHeavyAttack)
	{
		bSaveHeavyAttack = false;
		if(Combat->CombatState == ECombatState::ECState_HeavyAttack)
		{
			HeavyAttackEvent();
			SetStateInCode(ECombatState::ECState_Unoccupied);
		}
		HeavyAttackEvent();
	}
}
void ABMCharacter::ServerEquipButtonPressed_Implementation()
{
	if(Combat)
	{
		if(OverlappingWeapon)
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else if (Combat->bShouldSwapWeapon())
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
	AbilitySystemComponent = BattleMagePlayerState->GetAbilitySystemComponent();
	AttributeSet = BattleMagePlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent = BattleMagePlayerState->GetAbilitySystemComponent();AttributeSet = BattleMagePlayerState->GetAttributeSet();
	
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
