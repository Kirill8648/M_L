// Copyright 2021, Kirill Aksenov, Kirill Spiridonov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MLProjectile.generated.h"

UCLASS()
class MERCLIFE_API AMLProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMLProjectile();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MLProjectile")
	class UProjectileMovementComponent* ProjectileMovement;
};
