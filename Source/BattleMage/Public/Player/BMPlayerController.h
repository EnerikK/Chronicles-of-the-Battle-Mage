//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BMPlayerController.generated.h"

class UBMInputConfig;
struct FGameplayTag;
class UBMAbilitySystemComponent;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABMPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	
	UBMAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UBMInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UBMAbilitySystemComponent> AuraAbilitySystemComponent;

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> BMContest;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftPressed;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SlideAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> WeaponSwapAction;
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Crouch(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void SlideReleased(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Swap(const FInputActionValue& Value);

	
};
