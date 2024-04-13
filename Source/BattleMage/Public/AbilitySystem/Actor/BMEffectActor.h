// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "BMEffectActor.generated.h"

class USphereComponent;
struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class BATTLEMAGE_API ABMEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABMEffectActor();

protected:
	virtual void BeginPlay() override;

		
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap (AActor* TargetActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	bool bApplyEffectsToEnemies = false;
	
	//Instant Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//Duration effect
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;


	//Infinite Effect
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	//Removal Infinite
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;


	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Applied Effects")
	float ActorLevel = 1.f;

	
	
	
};
