// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "MLEngineSubsystem.h"
#include "Characters/Abilities/MLAbilitySystemGlobals.h"

void UMLEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UMLAbilitySystemGlobals::Get().InitGlobalData();
}
