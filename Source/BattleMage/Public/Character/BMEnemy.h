// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BMCharacterBase.h"
#include "HUD/Widget/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "BMEnemy.generated.h"

class UBmCharacterMovementComponent;
class ABattleMageAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMEnemy : public ABMCharacterBase , public IEnemyInterface
{
	GENERATED_BODY()

public:
	ABMEnemy(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;

	/*EnemyInterface*/
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	/*End EnemyInterface*/

	/*Combat Interface*/
	virtual int32 GetPlayerLevel();

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABattleMageAIController> BattleMageAIController;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharcterClassDefaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharcterClassDefaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category=Movement,meta=(AllowPrivateAccess = true))
	UBmCharacterMovementComponent* BMCharacterMovementComponent;
	
};
