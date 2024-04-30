// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"
#include "BMAttackSpell.generated.h"

class AWeapon;
class ABMProjectile;
class UBMChargeAttack;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMAttackSpell : public UBMDamageGameplayAbility
{
	GENERATED_BODY()
	

protected:

	UPROPERTY()
	ABMCharacter* PlayerCharacter;

	UPROPERTY()
	AWeapon* Weapon;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& HitTarget,const FVector& ProjectileTargetLocation,const FGameplayTag& SocketTag,bool bOverridePitch = false,float PitchOverride = 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABMProjectile> ProjectileClass;
	
	
};
