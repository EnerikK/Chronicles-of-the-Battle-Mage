// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UWidgetComponent;
class USphereComponent;
class ABMPlayerController;
class ABMCharacter;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EW_Initial				UMETA(DisplayName = "InitialState"),
	EW_Equipped				UMETA(DisplayName = "Equipped"),
	EW_Dropped				UMETA(DisplayName = "Dropped"),
	EW_EquippedSecondary	UMETA(DisplayName = "EquippedSecondary"),
	
	EW_MAX					UMETA(DisplayName = "DefaultMAX")
};
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EW_Sword      	UMETA(DisplayName = "Sword"),
	EW_Staff   		UMETA(DisplayName = "Staff"),
	EW_Daggers		UMETA(DisplayName = "Daggers"),
	EW_Spear		UMETA(DisplayName = "Spaer"),
	EW_Hook			UMETA(DisplayName = "Hook"),
	
	EF_MAX			UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BATTLEMAGE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	FVector GetSocketLocation(FName SocketName);
	virtual void Tick(float DeltaTime) override;
	void ShowPickUpWidget(bool bShowWidget);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetWeaponState(EWeaponState State);
	virtual void OnRep_Owner() override;
	virtual void Dropped();

	FORCEINLINE USphereComponent* GetPickUpSphere() const {return PickUpSphere;}
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()const {return WeaponMesh;}

	
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage* > AttackMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage* > HeavyAttackMontages;

	
	/*
	* Texture for the cross-hairs
	*/
	UPROPERTY(EditAnywhere,Category="Crosshair")
	UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere,Category="Crosshair")
	UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere,Category="Crosshair")
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere,Category="Crosshair")
	UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere,Category="Crosshair")
	UTexture2D* CrosshairBottom;
	/*
	* Texture for the cross-hairs end
	*/

protected:

	UPROPERTY()
	ABMCharacter* PlayerCharacter;

	UPROPERTY()
	ABMPlayerController* PlayerController;
	
	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,
		bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	USphereComponent* PickUpSphere;
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState,VisibleAnywhere,Category="Weapon Properties")
	EWeaponState WeaponState;
	
	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	UWidgetComponent* PickUpWidget;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
};
