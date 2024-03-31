// Hello :)


#include "Player/BMPlayerState.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"

ABMPlayerState::ABMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ABMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
