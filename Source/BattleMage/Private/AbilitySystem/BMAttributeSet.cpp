// Hello :)


#include "AbilitySystem/BMAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UBMAttributeSet::UBMAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}

void UBMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
}

void UBMAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Health,oldHealth);
}

void UBMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,MaxHealth,oldMaxHealth);

}

void UBMAttributeSet::OnRep_Mana(const FGameplayAttributeData& oldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Mana,oldMana);

}

void UBMAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,MaxMana,oldMaxMana);

}
