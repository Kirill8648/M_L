// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Characters/MLCharacterBase.h"
#include "MLHeroCharacter.generated.h"

/**
 * A player or AI controlled hero character.
 */
UCLASS()
class MERCLIFE_API AMLHeroCharacter : public AMLCharacterBase
{
	GENERATED_BODY()

public:
	AMLHeroCharacter(const class FObjectInitializer& ObjectInitializer);
};
