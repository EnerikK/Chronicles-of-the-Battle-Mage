// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/BMHud.h"
#include "Types/CombatState.h"
#include "CombatComponent.generated.h"

#define TRACE 80000.f


class ABMCharacterBase;
class AWeapon;
class ABMHud;
class ABMPlayerController;
class ABMCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEMAGE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend ABMCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EquipWeapon(AWeapon* WeaponToEquip);
	bool bShouldSwapWeapon();
	void SwapWeapon();

	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing= OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

protected:
	virtual void BeginPlay() override;

	void TraceUnderCrosshair(FHitResult& TraceHitResult);

	void SetHudCrosshair(float DeltaTime);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void DropEquippedWeapon();
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachActorToBackPack(AActor* ActorToAttach);
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);


private: 

	UPROPERTY()
	ABMCharacter* Character;
	
	UPROPERTY()
	ABMPlayerController* Controller;
	
	UPROPERTY()
	ABMHud* Hud;
	
	UPROPERTY(ReplicatedUsing=OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECState_Unoccupied;
	
	UFUNCTION()
	void OnRep_CombatState();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	FVector HitTarget;

	FHUDPackage HUDPackage;

	float CrosshairVelocity;


		
};
