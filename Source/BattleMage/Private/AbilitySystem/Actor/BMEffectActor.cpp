// Hello :)


#include "AbilitySystem/Actor/BMEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Components/SphereComponent.h"

ABMEffectActor::ABMEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	
}
void ABMEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABMEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABMEffectActor::EndOverlap);
	
}
void ABMEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UBMAttributeSet* AuraAttributeSet = Cast<UBMAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBMAttributeSet::StaticClass()));

		UBMAttributeSet* MutableAuraAttributeSet = const_cast<UBMAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		MutableAuraAttributeSet->SetStamina(AuraAttributeSet->GetStamina() + 50.f);
		MutableAuraAttributeSet->SetMana(AuraAttributeSet->GetMana() + 50.f);
		Destroy();
	}
}
void ABMEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


