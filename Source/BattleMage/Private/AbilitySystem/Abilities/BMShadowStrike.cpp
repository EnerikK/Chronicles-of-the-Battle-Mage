// Hello :)


#include "AbilitySystem/Abilities/BMShadowStrike.h"

#include "Character/BMCharacter.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"


void UBMShadowStrike::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if(HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	}
}
void UBMShadowStrike::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerPlayerCharacter = Cast<ABMCharacter>(CurrentActorInfo->AvatarActor);
	}
}
void UBMShadowStrike::TraceFirstTarget()
{
	check(OwnerPlayerCharacter);

	/*if(OwnerPlayerCharacter->Implements<UCombatInterface>())
	{
		TArray<AActor*> ActorsToIgnore;
     	ActorsToIgnore.Add(OwnerPlayerCharacter);
     	FHitResult HitResult;
     
     	UKismetSystemLibrary::SphereTraceSingle(
     	OwnerPlayerCharacter,
     	OwnerPlayerCharacter->GetActorLocation(),
     	(OwnerPlayerCharacter->GetActorForwardVector() * 2000.f) + OwnerPlayerCharacter->GetActorLocation() ,
     	30.f,
     	TraceTypeQuery1,
     	false,
     	ActorsToIgnore,
     	EDrawDebugTrace::Persistent,
     	HitResult,
     	true);
	}*/
	
	
}


