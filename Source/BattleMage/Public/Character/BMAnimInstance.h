// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/TurnInPlace.h"
#include "BMAnimInstance.generated.h"

class AWeapon;
class ABMCharacter;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UPROPERTY()
	AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	ABMCharacter* Character;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	float YawOffSet;

	UPROPERTY(BlueprintReadWrite,Category="Character",meta=(AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(BlueprintReadOnly,Category="Movement",meta=(AllowPrivateAccess = "true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly,Category="Character",meta=(AllowPrivateAccess = "true"))
	ETurnInPlace TurnInPlace;
	
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
	
	
	
	
};
