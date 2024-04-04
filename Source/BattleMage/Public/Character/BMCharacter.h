//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Character/BMCharacterBase.h"
#include "HUD/BMHud.h"
#include "BMCharacter.generated.h"

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

	ABMCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipButtonPressed();
	
	AWeapon* GetEquippedWeapon();

protected:

	UPROPERTY()
	ABMPlayerController* PlayerController;
	
	UPROPERTY()
	ABMCharacter* Character;
	
	UPROPERTY()
	ABMHud* Hud;

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
	
};
