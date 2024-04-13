// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "BMInputConfig.h"
#include "EnhancedInputComponent.h"
#include "BMInputComponent.generated.h"

struct FBattleMageInputAction;
class UBMInputConfig;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UBMInputConfig* InputConfig,
		UserClass* Object ,PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc,HeldFuncType HeldFunc);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UBMInputComponent::BindAbilityActions(const UBMInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FBattleMageInputAction& Action : InputConfig->AbilityInputAction)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
