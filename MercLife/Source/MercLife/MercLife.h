// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class EMLAbilityInputID : uint8
{
	// 0 None
	None UMETA(DisplayName = "None"),

	// 1 Confirm
	Confirm UMETA(DisplayName = "Confirm"),

	// 2 Cancel
	Cancel UMETA(DisplayName = "Cancel"),

	// 3 Jump
	Jump UMETA(DisplayName = "Jump"),

	// 4 Sprint
	Sprint UMETA(DisplayName = "Sprint"),

	// 5 RightHand Attack / BothHands ADS
	RightHandAction UMETA(DisplayName = "RightHand Attack / BothHands ADS"),

	// 6 LeftHand Attack / BothHands Attack
	LeftHandAction UMETA(DisplayName = "LeftHand Attack / BothHands Attack"),

	// 7 Next Weapon
	NextWeapon UMETA(DisplayName = "Next Weapon"),

	// 8 Previous Weapon
	PrevWeapon UMETA(DisplayName = "Previous Weapon")
};
