//Hello :)


#include "Character/BMCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "BMGameplayTags.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Player/BMPlayerState.h"

ABMCharacterBase::ABMCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	const FBattleMageGameplayTags& GameplayTags = FBattleMageGameplayTags::Get();

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}
void ABMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ABMCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABMCharacterBase::InitAbilityActorInfo()
{
	ABMPlayerState* BattleMagePlayerState = GetPlayerState<ABMPlayerState>();
	check(BattleMagePlayerState)
	BattleMagePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BattleMagePlayerState,this);
	Cast<UBMAbilitySystemComponent>(BattleMagePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = BattleMagePlayerState->GetAbilitySystemComponent();
	AttributeSet = BattleMagePlayerState->GetAttributeSet();
	
}


