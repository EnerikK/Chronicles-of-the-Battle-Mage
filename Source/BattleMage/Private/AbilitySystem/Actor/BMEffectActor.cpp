// Hello :)


#include "AbilitySystem/Actor/BMEffectActor.h"

ABMEffectActor::ABMEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABMEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABMEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABMEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ABMEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

