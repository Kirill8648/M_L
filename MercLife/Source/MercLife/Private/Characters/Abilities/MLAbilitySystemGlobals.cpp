// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Abilities/MLAbilitySystemGlobals.h"

UMLAbilitySystemGlobals::UMLAbilitySystemGlobals()
{
}

void UMLAbilitySystemGlobals::InitGlobalTags()
{
	Super::InitGlobalTags();

	DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
	NoWeaponTag = FGameplayTag::RequestGameplayTag("Weapon.Equipped.None");
	/*NoWeaponTagLeftTag = FGameplayTag::RequestGameplayTag("Weapon.Equipped.Left.None");
	NoWeaponTagBothTag = FGameplayTag::RequestGameplayTag("Weapon.Equipped.Both.None");*/
}