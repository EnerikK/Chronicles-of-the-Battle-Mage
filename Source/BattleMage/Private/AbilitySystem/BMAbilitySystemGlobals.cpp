// Hello :)


#include "AbilitySystem/BMAbilitySystemGlobals.h"
#include "BMAbilityTypes.h"

FGameplayEffectContext* UBMAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBMGameplayEffectContext();
}
