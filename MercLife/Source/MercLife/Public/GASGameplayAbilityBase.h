// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MercLife/MercLife.h"
#include "GASGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class MERCLIFE_API UGASGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASGameplayAbilityBase() {}

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EMLAbilityInputID AbilityInputID = EMLAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EMLAbilityInputID AbilityID = EMLAbilityInputID::None;
};
