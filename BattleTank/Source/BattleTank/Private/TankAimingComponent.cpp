// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // TODO Should this really tick

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) return;

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

		UE_LOG(LogTemp, Warning, TEXT("Barrel AimDirection"), *AimDirection.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No aim solve found"));
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Find Desired Pitch
	// LERP from current pitch to desired pitch over x seconds

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	UE_LOG(LogTemp, Warning, TEXT("%s DeltaRotator: %s"), *(GetOwner()->GetName()), *DeltaRotator.ToString());

	Barrel->Elevate(DeltaRotator.Pitch); // TODO remove magic number
}