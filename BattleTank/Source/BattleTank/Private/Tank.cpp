// Copyright Patrick Dugan

#include "Tank.h"
#include "TankAimingComponent.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto TankName = GetName();
	UE_LOG(LogTemp, Warning, TEXT("ATank::ATank %s"), *TankName);
}

void ATank::BeginPlay()
{
	// Needed for Blueprint BeginPlay to run
	Super::BeginPlay();

	auto TankName = GetName();
	UE_LOG(LogTemp, Warning, TEXT("ATank::BeginPlay %s"), *TankName);
}

UFUNCTION(BlueprintCallable, Category = "Setup")
void ATank::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
}

void ATank::Fire()
{
	if (!ensure(Barrel)) { return; }

	bool bIsReloaded = FPlatformTime::Seconds() - LastFireTime > ReloadTimeInSeconds;
	if (bIsReloaded)
	{
		// Spawn a projectile at the barrel location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		if (Projectile)
		{
			Projectile->LaunchProjectile(LaunchSpeed);
		}

		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::AimAt(FVector HitLocation)
{
	if (!ensure(TankAimingComponent)) return;

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}
