// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BMCharacterBase.h"
#include "HUD/Widget/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "BMEnemy.generated.h"

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

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharcterClassDefaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharcterClassDefaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
};
