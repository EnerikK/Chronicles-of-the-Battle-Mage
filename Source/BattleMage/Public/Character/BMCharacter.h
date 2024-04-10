//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "BMAnimInstance.h"
#include "Character/BMCharacterBase.h"
#include "HUD/BMHud.h"
#include "Types/TurnInPlace.h"
#include "BMCharacter.generated.h"

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
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipButtonPressed();
	
	void RotateInPlace(float DeltaTime);
	
	AWeapon* GetEquippedWeapon();
	bool IsWeaponEquipped();

	bool IsCrouching();
	bool IsSliding();

	UPROPERTY()
	FSlideStartDelegate SlideStartDelegate;
	
	UPROPERTY(EditAnywhere , Category= "Combat")
	UAnimMontage* SlideMontage;

	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return  AO_Pitch;}
	FORCEINLINE ETurnInPlace GetTurningInPlace() const {return TurningInPlace;}
	FORCEINLINE UBmCharacterMovementComponent* GetBMCharacterComponent() const {return BMCharacterMovementComponent;}

protected:
	
	UPROPERTY()
	ABMPlayerController* PlayerController;
	
	UPROPERTY()
	ABMCharacter* Character;
	
	UPROPERTY()
	ABMHud* Hud;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category=Movement,meta=(AllowPrivateAccess = true))
	UBmCharacterMovementComponent* BMCharacterMovementComponent;

	virtual void BeginPlay() override;

	void AimOffset(float DeltaTime);

private:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	UCombatComponent* Combat;

	UFUNCTION(Server,Reliable)
	void ServerEquipButtonPressed();
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	ETurnInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
};
