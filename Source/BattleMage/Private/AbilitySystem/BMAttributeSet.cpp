// Hello :)


#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BMGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"

UBMAttributeSet::UBMAttributeSet()
{
	
	const FBattleMageGameplayTags& GameplayTags = FBattleMageGameplayTags::Get();
	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);	
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

}

void UBMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*Vital*/
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Stamina,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,MaxStamina,COND_None,REPNOTIFY_Always);

	/*Primary*/
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Agility,COND_None,REPNOTIFY_Always);

	/*Secondary*/
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet,StaminaRegeneration,COND_None,REPNOTIFY_Always);
	
}

void UBMAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}
void UBMAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}
void UBMAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//Source = Cause of the effect , Target = Target of the effect ( owner of this )

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController =Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
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

void UBMAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Strength,OldStrength);

}

void UBMAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Intelligence,OldIntelligence);

}

void UBMAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Resilience,OldResilience);

}

void UBMAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Vigor,OldVigor);

}

void UBMAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Agility,OldAgility);
}

void UBMAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,Armor,OldArmor);

}

void UBMAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,ArmorPenetration,OldArmorPenetration);

}

void UBMAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,BlockChance,OldBlockChance);

}

void UBMAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,CriticalHitChance,OldCriticalHitChance);

}

void UBMAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,CriticalHitDamage,OldCriticalHitDamage);

}

void UBMAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,CriticalHitResistance,OldCriticalHitResistance);

}

void UBMAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,HealthRegeneration,OldHealthRegeneration);

}

void UBMAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,ManaRegeneration,OldManaRegeneration);

}

void UBMAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet,StaminaRegeneration,OldStaminaRegeneration);

}

