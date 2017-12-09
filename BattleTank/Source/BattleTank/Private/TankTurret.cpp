// Copyright Patrick Dugan

#include "TankTurret.h"


void UTankTurret::Rotate(float RelativeSpeed)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RelativeSpeed: %f"), *(GetOwner()->GetName()), RelativeSpeed);

	// Move the barrel the right amount this frame
	// Given a max elevation speed and the frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RelativeSpeed: %f"), *(GetOwner()->GetName()), RelativeSpeed);

	//UE_LOG(LogTemp, Warning, TEXT("MaxDegreesPerSecond: %f, DeltaSeconds %f"), MaxDegreesPerSecond, GetWorld()->GetDeltaSeconds());

	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->GetDeltaSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RotationChange: %f"), *(GetOwner()->GetName()), RotationChange);

	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	//UE_LOG(LogTemp, Warning, TEXT("%s UTankBarrel::Elevate RawNewElevation: %f"), *(GetOwner()->GetName()), RawNewRotation);

	//auto FinalElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	//UE_LOG(LogTemp, Warning, TEXT("%s FinalElevation %f"), *(GetOwner()->GetName()), FinalElevation);

	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}