// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MLAT_TickAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTickDelegate);

/**
 * Simple executes OnTick every tick.
 * NOTE: MUST BE ENDED MANUALLY
 */
UCLASS()
class MERCLIFE_API UMLAT_TickAbilityTask : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(BlueprintAssignable)
	FTickDelegate OnTick;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMLAT_TickAbilityTask* TickAbilityTask(UGameplayAbility* OwningAbility);

	//virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	//virtual void OnDestroy(bool bInOwnerFinished) override;
};
