// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RelativeSpeed: %f"), *(GetOwner()->GetName()), RelativeSpeed);

	// Move the barrel the right amount this frame
	// Given a max elevation speed and the frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RelativeSpeed: %f"), *(GetOwner()->GetName()), RelativeSpeed);

	//UE_LOG(LogTemp, Warning, TEXT("MaxDegreesPerSecond: %f, DeltaSeconds %f"), MaxDegreesPerSecond, GetWorld()->GetDeltaSeconds());

	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->GetDeltaSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate ElevatationChange: %f"), *(GetOwner()->GetName()), ElevationChange);

	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RawNewElevation: %f"), *(GetOwner()->GetName()), RawNewElevation);

	auto FinalElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	//UE_LOG(LogTemp, Warning, TEXT("%s FinalElevation %f"), *(GetOwner()->GetName()), FinalElevation);

	SetRelativeRotation(FRotator(FinalElevation, 0, 0));
}




