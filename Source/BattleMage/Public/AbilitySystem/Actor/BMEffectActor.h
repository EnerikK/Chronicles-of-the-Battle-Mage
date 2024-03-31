// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMEffectActor.generated.h"


class USphereComponent;

UCLASS()
class BATTLEMAGE_API ABMEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABMEffectActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh;
	
};
