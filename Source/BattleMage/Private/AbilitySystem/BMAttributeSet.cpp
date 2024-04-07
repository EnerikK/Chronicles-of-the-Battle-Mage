// Hello :)


#include "AbilitySystem/BMAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UBMAttributeSet::UBMAttributeSet()
{
	InitHealth(150.f);
	InitMaxHealth(200.f);
	InitMana(80.f);
	InitMaxMana(200.f);
	InitStamina(100.f);
	InitMaxStamina(200.f);
}

void UBMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Stamina,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxStamina,COND_None,REPNOTIFY_Always);
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

void UBMAttributeSet::OnRep_Stamina(const FGameplayAttributeData& oldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Stamina,oldStamina);

}

void UBMAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,MaxStamina,oldMaxStamina);
}
