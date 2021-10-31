// Copyright 2021, Kirill Aksenov, Kirill Spiridonov


#include "Weapons/MLProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMLProjectile::AMLProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 7000.0f;
}
