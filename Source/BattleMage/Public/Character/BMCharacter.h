//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "BMAnimInstance.h"
#include "BMCharacterMovementComponent.h"
#include "Character/BMCharacterBase.h"
#include "HUD/BMHud.h"
#include "Types/TurnInPlace.h"
#include "BMCharacter.generated.h"

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
	void AttackButtonPressed(int32 IncrementAttack);
	AWeapon* GetEquippedWeapon();
	bool IsWeaponEquipped();
	bool CanAttack();

	void Attack();
	void IncrementAttack(int32 CurrentAttack = 0);
	void PlayAttackMontage(int32 CurrentAttack = 0);
	
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
	
};
