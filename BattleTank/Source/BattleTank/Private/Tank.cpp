// Copyright Patrick Dugan

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto TankName = GetName();
}

void ATank::BeginPlay()
{
	// Needed for Blueprint BeginPlay to run
	Super::BeginPlay();
}

UFUNCTION(BlueprintCallable, Category = "Setup")
void ATank::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
}

void ATank::Fire()
{
	UE_LOG(LogTemp, Error, TEXT("ATank::Fire"));
}
