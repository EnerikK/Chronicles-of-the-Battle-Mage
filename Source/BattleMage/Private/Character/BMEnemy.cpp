// Hello :)


#include "Character/BMEnemy.h"
#include "BMGameplayTags.h"
#include "AbilitySystem/BlueprintSystemLibrary.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "AI/BattleMageAIController.h"
#include "BattleMage/BattleMage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

ABMEnemy::ABMEnemy(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UBmCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	BMCharacterMovementComponent = Cast<UBmCharacterMovementComponent>(GetCharacterMovement());
	BMCharacterMovementComponent->SetIsReplicated(true);
	BMCharacterMovementComponent->bUseControllerDesiredRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>("Attribute");
}

void ABMEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
		
	BattleMageAIController = Cast<ABattleMageAIController>(NewController);
	BattleMageAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BattleMageAIController->RunBehaviorTree(BehaviorTree);
	BattleMageAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReact"),false);
}


void ABMEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	BMCharacterMovementComponent->MaxWalkSpeed = BaseWalkSpeed;
	if(HasAuthority())
	{
		UBlueprintSystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent);
	}
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
		AbilitySystemComponent->RegisterGameplayTagEvent(FBattleMageGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,&ABMEnemy::HitReactTagChanged);
		OnHealthChanged.Broadcast(BMAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BMAS->GetMaxHealth());
	}
}
void ABMEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	BMCharacterMovementComponent->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	BattleMageAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReact"),bHitReacting);

}

void ABMEnemy::InitAbilityActorInfo()
{
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UBMAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
	}

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

