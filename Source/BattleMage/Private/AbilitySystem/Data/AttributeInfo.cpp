// Hello :)


#include "AbilitySystem/Data/AttributeInfo.h"

FBattleMageAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
	bool bLogNotFound) const
{
	for(const FBattleMageAttributeInfo& Info : AttributeInformation)
	{
		if(Info.AttributeTag.MatchesTag(AttributeTag))
		{
			return Info;
		}
		
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Cant find info for attributeTag [%s] on attributeinfo [%s]."),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FBattleMageAttributeInfo();
}
