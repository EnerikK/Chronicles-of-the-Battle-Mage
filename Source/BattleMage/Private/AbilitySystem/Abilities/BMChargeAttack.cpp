// Hello :)


#include "AbilitySystem/Abilities/BMChargeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BMGameplayTags.h"
#include "Character/BMCharacter.h"
#include "Interaction/CombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/Weapon.h"

void UBMChargeAttack::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		PlayerCharacter = Cast<ABMCharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBMChargeAttack::GenerateAttack(const FGameplayTag& SocketTag,
                                     bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 3);
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer)return;

	AWeapon* EquippedWeapon = Cast<AWeapon>(PlayerCharacter->GetEquippedWeapon());

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface && EquippedWeapon)
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
		EquippedWeapon->DamageEffectSpecHandle = SpecHandle;
		
		const FBattleMageGameplayTags GameplayTags = FBattleMageGameplayTags::Get();

		
	}
	
}

