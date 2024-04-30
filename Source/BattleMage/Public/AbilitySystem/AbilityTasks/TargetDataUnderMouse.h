// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

class ABMCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&,
                                            DataHandle);

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,Category="Ability | Tasks",meta= (DisplayName = " TargetDataUnderMouse" ,HidePin="OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "True"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:

	FVector HitTarget;

	UPROPERTY()
	ABMCharacter* ControlledCharacter;

	virtual void Activate() override;
	void SendTraceUnderCursorData(FHitResult& TraceHitResult);

	void OnTargetDataReplicatedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
	
};
