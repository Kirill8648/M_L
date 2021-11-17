// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "GASGameplayAbilityBase.h"
#include "Characters/Heroes/MLHeroCharacter.h"

UGASGameplayAbilityBase::UGASGameplayAbilityBase()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bSourceObjectMustEqualCurrentWeaponToActivate = false;
}

bool UGASGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* GameplayTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (bSourceObjectMustEqualCurrentWeaponToActivate)
	{
		AMLHeroCharacter* Hero = Cast<AMLHeroCharacter>(ActorInfo->AvatarActor);
		if (Hero && Hero->GetCurrentWeapon() && (UObject*)Hero->GetCurrentWeapon() == GetSourceObject(Handle, ActorInfo))
		{
			return Super::CanActivateAbility(Handle, ActorInfo, GameplayTags, TargetTags, OptionalRelevantTags);
		}
		else
		{
			return false;
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, GameplayTags, TargetTags, OptionalRelevantTags);
}
