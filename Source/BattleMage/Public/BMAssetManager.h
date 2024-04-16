// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BMAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UBMAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
