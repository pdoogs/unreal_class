// Copyright Patrick Dugan

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Public/DrawDebugHelpers.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	// So that first fire is after initial reload
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (FiringState == EFiringState::OutOfAmmo)
	{
		return;
	}
	else if (FPlatformTime::Seconds() - LastFireTime < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}


void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

bool UTankAimingComponent::IsBarrelMoving() const
{
	if (!ensure(Barrel)) { return false; }
	return !Barrel->GetForwardVector().Equals(AimDirection, 0.01f);
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel || Turret)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// calculate hte out launch velocity
	const bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace); // Without providing DoNotTrace SuggestProjectileVelocity will produce inconsistent results

	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();

		MoveBarrelTowards(AimDirection);

		//UE_LOG(LogTemp, Warning, TEXT("%s Barrel AimDirection %s"), *(GetOwner()->GetName()), *AimDirection.ToString());
		/*
		DrawDebugLine(
			GetWorld(),
			StartLocation,
			StartLocation + 1000 * AimDirection,
			FColor(255, 0, 0),
			false, -1, 0,
			12.333
		);
		*/
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirectionIn)
{
	if (!ensure(Barrel && Turret)) return;
	// Find Desired Pitch

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirectionIn.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	auto Yaw = DeltaRotator.Yaw;
	if (Yaw < -180 || Yaw > 180)
	{
		Yaw *= -1;
	}
	Turret->Rotate(Yaw);

	// Debug Projectile Line
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto BarrelDir = BarrelRotation.Vector().GetSafeNormal();
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		StartLocation + 1000 * BarrelDir,
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
	);

}

void UTankAimingComponent::Fire()
{
	if (FiringState != EFiringState::Reloading && 
		FiringState != EFiringState::OutOfAmmo)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		// Spawn a projectile at the barrel location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		// Debug Draw Firing Path
		/*
		auto LineStart = Barrel->GetSocketLocation(FName("Projectile"));
		auto LineDir = Barrel->GetSocketRotation(FName("Projectile")).Vector().GetSafeNormal();
		DrawDebugLine(
			GetWorld(),
			LineStart,
			LineStart + 1000 * LineDir,
			FColor(0, 0, 0xff),
			true);
			*/

		if (Projectile)
		{
			Projectile->LaunchProjectile(LaunchSpeed);
			RoundsLeft--;

			if (RoundsLeft <= 0)
			{
				RoundsLeft = 0;
				FiringState = EFiringState::OutOfAmmo;
			}
		}
		LastFireTime = FPlatformTime::Seconds();
	}
}

