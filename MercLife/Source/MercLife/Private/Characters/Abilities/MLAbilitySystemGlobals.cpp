// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Abilities/MLAbilitySystemGlobals.h"

UMLAbilitySystemGlobals::UMLAbilitySystemGlobals()
{
}

void UMLAbilitySystemGlobals::InitGlobalTags()
{
	Super::InitGlobalTags();

	DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
	WeaponAbilityTag = FGameplayTag::RequestGameplayTag("Ability.Weapon");
	//AbilityJumpTag = FGameplayTag::RequestGameplayTag("Ability.Jump");
	/*NoWeaponTagLeftTag = FGameplayTag::RequestGameplayTag("Weapon.Equipped.Left.None");
	NoWeaponTagBothTag = FGameplayTag::RequestGameplayTag("Weapon.Equipped.Both.None");*/
}
