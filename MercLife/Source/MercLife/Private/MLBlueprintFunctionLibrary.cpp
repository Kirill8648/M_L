// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "MLBlueprintFunctionLibrary.h"

FVector2D UMLBlueprintFunctionLibrary::RandPointInEllipse(float Width, float Height)
{
	const FVector2D PointInCircle = FMath::RandPointInCircle(1.f);
	return FVector2D(PointInCircle.X * Width, PointInCircle.Y * Height);
}
