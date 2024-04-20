//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "BMAnimInstance.h"
#include "BMCharacterMovementComponent.h"
#include "Character/BMCharacterBase.h"
#include "HUD/BMHud.h"
#include "Types/CombatState.h"
#include "Types/TurnInPlace.h"
#include "BMCharacter.generated.h"

enum class ECombatState : uint8;
class ABMPlayerState;
class UBmCharacterMovementComponent;
class AWeapon;
class ABMPlayerController;
class UCombatComponent;
class ABMHud;
class USpringArmComponent;
class UCameraComponent;
class UTexture2D;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMCharacter : public ABMCharacterBase
{
	GENERATED_BODY()

public:

	ABMCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;

	/*Weapons*/
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipButtonPressed();
	AWeapon* GetEquippedWeapon();
	bool IsWeaponEquipped();
	UPROPERTY(BlueprintReadWrite)
	bool bFinishedSwapping = true;
	void PlaySwapMontage();

	/*States*/
	ECombatState CurrentState;
	UFUNCTION(BlueprintCallable)
	void SetStateInCode(ECombatState NewState);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsStateEqualToAnyInCode(TArray<ECombatState> StatesToCheck);
	/*Attack*/
	UPROPERTY(BlueprintReadWrite)
	int32 AttackIndexInCode = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 HeavyAttackIndexInCode = 0;
	UPROPERTY(BlueprintReadWrite)
	bool bSaveLightAttack = false;
	UPROPERTY(BlueprintReadWrite)
	bool bSaveHeavyAttack = false;
	UFUNCTION(BlueprintCallable)
	bool PerformLightAttackInCode(int32 CurrentAttackIndex);
	UFUNCTION(BlueprintCallable)
	bool PerformHeavyAttackInCode(int32 CurrentAttackIndex);
	UFUNCTION(BlueprintCallable)
	void AttackEvent();
	UFUNCTION(BlueprintCallable)
	void HeavyAttackEvent();
	UFUNCTION(BlueprintCallable)
	void ResetValues();
	UFUNCTION(BlueprintCallable)
	void SaveLightAttack();
	UFUNCTION(BlueprintCallable)
	void SaveHeavyAttack();
	void TimerBeforePressingAgain();
	bool IsAttacking = false;
	void SwapWeaponTimerFinished();
	
	/*Character*/
	void RotateInPlace(float DeltaTime);
	bool IsCrouching();
	bool IsSliding();
	bool bPressedBattleMageJump;
	virtual void Jump() override;
	virtual void StopJumping() override;

	/*Delegates*/
	UPROPERTY()
	FSlideStartDelegate SlideStartDelegate;
	
	/*Getters*/
	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return  AO_Pitch;}
	FORCEINLINE ETurnInPlace GetTurningInPlace() const {return TurningInPlace;}
	FORCEINLINE UBmCharacterMovementComponent* GetBMCharacterComponent() const {return BMCharacterMovementComponent;}
	FORCEINLINE UAnimMontage* GetSlideMontage() const {return SlideMontage;}
	FORCEINLINE UAnimMontage* GetSwordPickUpMontage() const {return SwordPickUpMontage;}
	FORCEINLINE UAnimMontage* GetWeaponSwapMontage() const {return WeaponSwapMontage;}


protected:

	virtual void BeginPlay() override;
	void AimOffset(float DeltaTime);
	
	UPROPERTY()
	ABMPlayerController* PlayerController;
	
	UPROPERTY()
	ABMCharacter* Character;
	
	UPROPERTY()
	ABMHud* Hud;

	UPROPERTY()
	ABMPlayerState* BMPlayerState;

private:

	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	UCombatComponent* Combat;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category=Movement,meta=(AllowPrivateAccess = true))
	UBmCharacterMovementComponent* BMCharacterMovementComponent;
	
	UFUNCTION(Server,Reliable)
	void ServerEquipButtonPressed();
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/*Turn In place*/
	ETurnInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* SlideMontage;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* SwordPickUpMontage;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* WeaponSwapMontage;
	
};
