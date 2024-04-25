// Hello :)


#include "Player/BMPlayerState.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Player/BMPlayerController.h"


ABMPlayerState::ABMPlayerState()
{
	AActor::SetOwner(Controller);

	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ABMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABMPlayerState,Level);
}

UAbilitySystemComponent* ABMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABMPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ABMPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}
