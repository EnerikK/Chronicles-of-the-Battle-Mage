//Hello :)


#include "Character/BMCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "BMGameplayTags.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/BMPlayerState.h"
#include "Weapon/Weapon.h"

class UBmCharacterMovementComponent;

ABMCharacterBase::ABMCharacterBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	const FBattleMageGameplayTags& GameplayTags = FBattleMageGameplayTags::Get();
}
void ABMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
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
	if(IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(WeaponTipSocketName);

	}
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

void ABMCharacterBase::Die()
{
	MulticastHandleDeath();
}

void ABMCharacterBase::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABMCharacterBase::InitAbilityActorInfo()
{
}

void ABMCharacterBase::AddCharacterAbilities()
{
	UBMAbilitySystemComponent* BMASC = CastChecked<UBMAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;
	BMASC->AddCharacterAbilities(StartUpAbilities);
}

void ABMCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void ABMCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}


