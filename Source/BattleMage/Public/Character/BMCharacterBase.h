//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BMCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BMCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class BATTLEMAGE_API ABMCharacterBase : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	ABMCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* Attributes() const {return AttributeSet;}

	/*Combat Interface*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	/*CombatInterface End*/

	FOnASCRegistered OnAscRegistered;

	
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	void AddCharacterAbilities();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere,Category="Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly)
	FName RightHandSocket;

	bool bIsDead = false;

private:

	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
	

};
