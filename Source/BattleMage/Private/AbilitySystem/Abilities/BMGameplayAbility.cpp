// Hello :)


#include "AbilitySystem/Abilities/BMGameplayAbility.h"

#include "AbilitySystem/BMAttributeSet.h"
#include "Character/BMCharacter.h"




float UBMGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(auto Mod : CostEffect->Modifiers)
		{
			if(Mod.Attribute == UBMAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UBMGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if(const UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,Cooldown);
	}
	return Cooldown;
}
