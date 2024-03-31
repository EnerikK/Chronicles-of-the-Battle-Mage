//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BMCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class BATTLEMAGE_API ABMCharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABMCharacterBase();
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* Attributes() const {return AttributeSet;}
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	virtual void InitAbilityActorInfo();

	UPROPERTY(EditDefaultsOnly)
	FName RightHandSocket;

	bool bIsDead = false;
	
	

};
