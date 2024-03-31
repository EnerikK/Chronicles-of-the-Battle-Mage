//Hello :)


#include "Character/BMCharacter.h"
#include "AbilitySystemComponent.h"
#include "MeshAttributeArray.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/BMPlayerState.h"

ABMCharacter::ABMCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ABMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();//Here we have the server actor info 
}

void ABMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();//meanwhile here we have the actor info for the client 
}

void ABMCharacter::InitAbilityActorInfo()
{
	ABMPlayerState* BattleMagePlayerState = GetPlayerStateChecked<ABMPlayerState>();
	check(BattleMagePlayerState);
	
	BattleMagePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BattleMagePlayerState,this);
	Cast<UBMAbilitySystemComponent>(BattleMagePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = BattleMagePlayerState->GetAbilitySystemComponent();AttributeSet = BattleMagePlayerState->GetAttributeSet();
	AttributeSet = BattleMagePlayerState->GetAttributeSet();
	
	
}
