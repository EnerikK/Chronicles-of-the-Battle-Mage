// Hello :)


#include "Character/BMEnemy.h"

#include "AbilitySystem/BlueprintSystemLibrary.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "BattleMage/BattleMage.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

ABMEnemy::ABMEnemy(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("Attribute");
}

void ABMEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if(UBMUserWidget* BMUserWidget = Cast<UBMUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BMUserWidget->SetWidgetController(this);
	}
	
	if (const UBMAttributeSet* BMAS = Cast<UBMAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BMAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BMAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		OnHealthChanged.Broadcast(BMAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BMAS->GetMaxHealth());
	}
}


void ABMEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UBMAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();

}

void ABMEnemy::InitializeDefaultAttributes() const
{
	UBlueprintSystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
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

int32 ABMEnemy::GetPlayerLevel()
{
	return Level;
}

