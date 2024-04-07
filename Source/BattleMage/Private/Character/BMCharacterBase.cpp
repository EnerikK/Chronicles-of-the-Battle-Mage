//Hello :)


#include "Character/BMCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "BMGameplayTags.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BMPlayerState.h"

class UBmCharacterMovementComponent;

ABMCharacterBase::ABMCharacterBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	BMCharacterMovementComponent = Cast<UBmCharacterMovementComponent>(GetCharacterMovement());

	const FBattleMageGameplayTags& GameplayTags = FBattleMageGameplayTags::Get();
	
}
void ABMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ABMCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ABMCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

FVector ABMCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FBattleMageGameplayTags& GameplayTags = FBattleMageGameplayTags::Get();
	
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);

	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);

	}
	return FVector();
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


