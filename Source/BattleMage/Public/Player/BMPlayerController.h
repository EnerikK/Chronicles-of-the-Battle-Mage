//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BMPlayerController.generated.h"

class UTextDamage;
class UCombatComponent;
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);


	UPROPERTY(Replicated)
	bool bDisableGameplay = false;
	FORCEINLINE bool GetDisableGameplay() const {return bDisableGameplay;}
	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UBMAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UBMInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UBMAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTextDamage> DamageTextComponentClass;

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
	TObjectPtr<UInputAction> HeavyAttackAction;
	
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
	void HeavyAttack(const FInputActionValue& Value);

	
};
