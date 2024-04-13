// Hello :)


#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
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


