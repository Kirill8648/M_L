// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MLBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MERCLIFE_API UMLBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Random")
	static FVector2D RandPointInEllipse(float Width, float Height);
};
