// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Characters/Abilities/AbilityTasks/MLAT_WaitDelayOneFrame.h"

UMLAT_WaitDelayOneFrame::UMLAT_WaitDelayOneFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMLAT_WaitDelayOneFrame::Activate()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMLAT_WaitDelayOneFrame::OnDelayFinish);
}

UMLAT_WaitDelayOneFrame* UMLAT_WaitDelayOneFrame::WaitDelayOneFrame(UGameplayAbility* OwningAbility)
{
	UMLAT_WaitDelayOneFrame* MyObj = NewAbilityTask<UMLAT_WaitDelayOneFrame>(OwningAbility);
	return MyObj;
}

void UMLAT_WaitDelayOneFrame::OnDelayFinish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}
