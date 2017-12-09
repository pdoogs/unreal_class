// Copyright Patrick Dugan

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Public/DrawDebugHelpers.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}
/*
void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret * TurretToSet)
{
	Turret = TurretToSet;
}
*/

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel || Turret)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	//UE_LOG(LogTemp, Warning, TEXT("StartLocation: %s"), *StartLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("EndLocation: %s"), *HitLocation.ToString());

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
		//UE_LOG(LogTemp, Warning, TEXT("OutLaunchVelocity: %s"), *OutLaunchVelocity.ToString());

		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("%s Aiming at %s"), *TankName, *AimDirection.ToString());

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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s No aim solve found"), *(GetOwner()->GetName()));
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) return;
	// Find Desired Pitch

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//UE_LOG(LogTemp, Warning, TEXT("%s BarrelRotator: %s, AimAsRotator: %s"), *(GetOwner()->GetName()), *BarrelRotator.ToString(), *AimAsRotator.ToString());

	//UE_LOG(LogTemp, Warning, TEXT("%s DeltaRotator: %s"), *(GetOwner()->GetName()), *DeltaRotator.ToString());

	Barrel->Elevate(DeltaRotator.Pitch);

	Turret->Rotate(DeltaRotator.Yaw);

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