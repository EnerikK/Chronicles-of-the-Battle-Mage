// Hello :)


#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"

#include "Interaction/CombatInterface.h"

void UBMDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	
}

FTaggedMontage UBMDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages) const
{
	if(TaggedMontages.Num()>0)
	{
		const int32 Selection = FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}

float UBMDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
