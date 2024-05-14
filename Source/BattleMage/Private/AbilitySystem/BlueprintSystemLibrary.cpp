// Hello :)


#include "AbilitySystem/BlueprintSystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "BMAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "Game/BMGameModeBase.h"
#include "HUD/BMHud.h"
#include "HUD/BMWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BMPlayerState.h"

bool UBlueprintSystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                         FWidgetControllerParams& OutWCParams, ABMHud*& OutBMHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutBMHUD = Cast<ABMHud>(PC->GetHUD());
		if (OutBMHUD)
		{
			ABMPlayerState* PS = PC->GetPlayerState<ABMPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UAttributeStatusWidget* UBlueprintSystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABMHud* BMHUD = Cast<ABMHud>(PC->GetHUD()))
		{
			ABMPlayerState* PS = PC->GetPlayerState<ABMPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BMHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UOverlayWidgetController* UBlueprintSystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABMHud* BMHUD = Cast<ABMHud>(PC->GetHUD()))
		{
			ABMPlayerState* PS = PC->GetPlayerState<ABMPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BMHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UBlueprintSystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ABMGameModeBase* AuraGameMode = Cast<ABMGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassInfoAttributes ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UBlueprintSystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	ABMGameModeBase* AuraGameMode = Cast<ABMGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for(TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UBlueprintSystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ABMGameModeBase* BMGameMode = Cast<ABMGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(BMGameMode == nullptr) return nullptr;
	return BMGameMode->CharacterClassInfo;
}

bool UBlueprintSystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBMGameplayEffectContext* BMEffectContext = static_cast<const FBMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BMEffectContext->IsBlockedHit();
	}
	return false;
}

bool UBlueprintSystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBMGameplayEffectContext* BMEffectContext = static_cast<const FBMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BMEffectContext->IsCriticalHit();
	}
	return false;
}

void UBlueprintSystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FBMGameplayEffectContext* BMEffectContext = static_cast<FBMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BMEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UBlueprintSystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	
	if (FBMGameplayEffectContext* BMEffectContext = static_cast<FBMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BMEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
