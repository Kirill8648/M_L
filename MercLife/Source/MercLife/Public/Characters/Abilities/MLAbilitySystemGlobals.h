// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MLAbilitySystemGlobals.generated.h"

/**
 * Child class of UAbilitySystemGlobals.
 * Do not try to get a reference to this or call into it during constructors of other UObjects. It will crash in packaged games.
 */
UCLASS()
class MERCLIFE_API UMLAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UMLAbilitySystemGlobals();

	/**
	* Cache commonly used tags here. This has the benefit of one place to set the tag FName in case tag names change and
	* the function call into UMLAbilitySystemGlobals::MLGet() is cheaper than calling FGameplayTag::RequestGameplayTag().
	* Classes can access them by UMLAbilitySystemGlobals::MLGet().DeadTag
	*/

	UPROPERTY()
	FGameplayTag DeadTag;
	UPROPERTY()
	FGameplayTag WeaponAbilityTag;
	/*UPROPERTY()
	FGameplayTag AbilityJumpTag;*/

	/*UPROPERTY()
	FGameplayTag NoWeaponTagLeftTag;

	UPROPERTY()
	FGameplayTag NoWeaponTagBothTag;*/

	static UMLAbilitySystemGlobals& MLGet()
	{
		return dynamic_cast<UMLAbilitySystemGlobals&>(Get());
	}

	/** TODO ALLOCATE A PROJECT SPECIFIC GameplayEffectContext STRUCT */
	//virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	virtual void InitGlobalTags() override;
};
