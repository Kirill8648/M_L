// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Abilities/AbilityTasks/MLAT_TickAbilityTask.h"

UMLAT_TickAbilityTask::UMLAT_TickAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UMLAT_TickAbilityTask* UMLAT_TickAbilityTask::TickAbilityTask(UGameplayAbility* OwningAbility)
{
	UMLAT_TickAbilityTask* MyObj = NewAbilityTask<UMLAT_TickAbilityTask>(OwningAbility);

	return MyObj;
}

/*void UMLAT_TickAbilityTask::Activate()
{
}*/

void UMLAT_TickAbilityTask::TickTask(float DeltaTime)
{
	OnTick.Broadcast();
	//Super::TickTask(DeltaTime);
}

/*void UMLAT_TickAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}*/
