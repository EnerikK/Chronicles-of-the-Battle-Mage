// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMGameplayAbility.h"
#include "BMDamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMDamageGameplayAbility : public UBMGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DeathImpulseMagnitude  = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float KnockbackForceMagnitude  = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float KnockbackChance = 0.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)const;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Damage")
	float RadialDamageOuterRadius = 0.f;
	
};
