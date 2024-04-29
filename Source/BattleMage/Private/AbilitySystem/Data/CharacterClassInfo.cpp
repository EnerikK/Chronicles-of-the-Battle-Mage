// Hello :)


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassInfoAttributes UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}