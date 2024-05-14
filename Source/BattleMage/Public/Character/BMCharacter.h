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

class UCollisionHandlerComponent;
class UBoxComponent;
class UBMMotionWarping;
class ABMPlayerState;
class UBmCharacterMovementComponent;
class AWeapon;
class ABMPlayerController;
class UCombatComponent;
class ABMHud;
class USpringArmComponent;
class UCameraComponent;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionDetected, const FHitResult&, HitResult);



#define TRACE 80000.f
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
	virtual void OnRep_ReplicatedMovement() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;

	/*Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void WeaponCollision_Implementation(AWeapon* CurrentWeapon,float Radius, float  , TArray<FHitResult>& outHits) override;
	FRotator GetLineRotation(FVector Start,FVector End);

	/*Weapons*/
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipButtonPressed();
	AWeapon* GetEquippedWeapon();
	bool IsWeaponEquipped();
	
	
	bool bFinishedSwapping = false;
	void PlaySwapMontage();
	
	ECombatState GetCombatState() const;
	

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
	void PerformLightAttackInCode(int32 CurrentAttackIndex);
	UFUNCTION(BlueprintCallable)
	bool PerformHeavyAttackInCode(int32 CurrentAttackIndex);
	UFUNCTION(BlueprintCallable)
	void AttackEvent();
	UFUNCTION(BlueprintCallable)
	void HeavyAttackEvent();
	UFUNCTION(BlueprintCallable)
	void SaveLightAttack();
	UFUNCTION(BlueprintCallable)
	void SaveHeavyAttack();
	
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
	UPROPERTY()
	FOnCollisionDetected OnCollisionDetected;
	
	/*Getters*/
	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return  AO_Pitch;}
	FORCEINLINE ETurnInPlace GetTurningInPlace() const {return TurningInPlace;}
	FORCEINLINE UBmCharacterMovementComponent* GetBMCharacterComponent() const {return BMCharacterMovementComponent;}
	FORCEINLINE UAnimMontage* GetSlideMontage() const {return SlideMontage;}
	FORCEINLINE UAnimMontage* GetSwordPickUpMontage() const {return SwordPickUpMontage;}
	FORCEINLINE UAnimMontage* GetWeaponSwapMontage() const {return WeaponSwapMontage;}
	FVector GetHitTarget() const;


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

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComponent;

private:

	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	UCombatComponent* Combat;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	UBMMotionWarping* BMMotionWarping;
	
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
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* SlideMontage;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* SwordPickUpMontage;

	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* WeaponSwapMontage;

	FVector HitTarget;
	
};
