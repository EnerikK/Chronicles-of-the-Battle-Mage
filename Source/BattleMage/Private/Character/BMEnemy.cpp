// Hello :)


#include "Character/BMEnemy.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"

ABMEnemy::ABMEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("Attribute");
}

void ABMEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}
