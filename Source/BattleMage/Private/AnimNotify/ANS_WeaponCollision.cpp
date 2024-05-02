// Hello :)


#include "AnimNotify/ANS_WeaponCollision.h"

#include "Kismet/KismetSystemLibrary.h"

void UANS_WeaponCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float FrameDeltaTime)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		FVector StartSocket = MeshComp->GetSocketLocation("WeaponStartSocket");
		FVector EndSocket = MeshComp->GetSocketLocation("WeaponEndSocket");
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MeshComp->GetOwner());
		TArray<FHitResult> HitArray;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
		objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),StartSocket,EndSocket,20.f,objectTypesArray,false,ActorsToIgnore,
		EDrawDebugTrace::ForDuration,HitArray,true,FLinearColor::Gray,
	FLinearColor::Blue,60.f);

		

	}
}
