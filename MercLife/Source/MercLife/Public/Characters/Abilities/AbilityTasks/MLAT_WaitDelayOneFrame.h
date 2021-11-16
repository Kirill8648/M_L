// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MLAT_WaitDelayOneFrame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDelayOneFrameDelegate);

/**
 * Like WaitDelay but only delays one frame (tick).
 */
UCLASS()
class MERCLIFE_API UMLAT_WaitDelayOneFrame : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitDelayOneFrameDelegate OnFinish;

	virtual void Activate() override;

	// Wait one frame.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMLAT_WaitDelayOneFrame* WaitDelayOneFrame(UGameplayAbility* OwningAbility);

private:
	void OnDelayFinish();
};
