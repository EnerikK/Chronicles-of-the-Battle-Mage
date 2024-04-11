// Hello :)


#include "Game/BattleMageCameraManager.h"
#include "Character/BMCharacter.h"
#include "Character/BMCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ABattleMageCameraManager::ABattleMageCameraManager()
{
}

void ABattleMageCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	
	if (ABMCharacter* BMCharacter = Cast<ABMCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UBmCharacterMovementComponent* BMC = BMCharacter->GetBMCharacterComponent();
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			BMC->GetCrouchedHalfHeight() - BMCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (BMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
