// Hello :)


#include "BMAssetManager.h"

#include "BMGameplayTags.h"

UBMAssetManager& UBMAssetManager::Get()
{
	check(GEngine);

	UBMAssetManager* AuraAssetManager = Cast<UBMAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UBMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FBattleMageGameplayTags::InitializeNativeGameplayTags();
}
