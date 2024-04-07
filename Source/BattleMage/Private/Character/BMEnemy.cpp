// Hello :)


#include "Character/BMEnemy.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "BattleMage/BattleMage.h"

ABMEnemy::ABMEnemy(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("Attribute");
}

void ABMEnemy::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ABMEnemy::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ABMEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}
