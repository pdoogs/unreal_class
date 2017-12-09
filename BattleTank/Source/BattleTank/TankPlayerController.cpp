// Copyright Patrick Dugan

#include "TankPlayerController.h"
#include "GameFramework/Actor.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) return;

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AimTowardCrosshair();
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!GetPawn()) return;

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation;
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	UE_LOG(LogTemp, Warning, TEXT("bGotHitLocation %i"), bGotHitLocation);

	// Get world location through crosshair (line trace)
	if (bGotHitLocation)
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector & HitLocation) const
{
	// Get PlayerUI object
	// Get AimPoint from Player UI

	// Project AimPoint into 3D Point
	// Cast Ray from AimPoint into world

	// If hit, update HitLocation and return true
	// Else Return false

	//auto PlayerController = GetWorld()->GetFirstPlayerController();
	//PlayerController->Find

	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenLocation(
		ViewportSizeX * CrosshairXLocation,
		ViewportSizeY * CrosshairYLocation);

	//UE_LOG(LogTemp, Warning, TEXT("Screen Location: %s"), *ScreenLocation.ToString());

	FVector LookDirection;
	FVector CameraLocation;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//UE_LOG(LogTemp, Warning, TEXT("LookDirection: %s"), *LookDirection.ToString());

		if (GetLookVectorHitLocation(HitLocation, LookDirection))
		{
			//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString());
			return true;
		}
		else
		{
			FVector ZeroVector(0.f);
			//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *ZeroVector.ToString());
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("LookDirection: %s"), *LookDirection.ToString());

	// Deproject screen position to world direction
	// Line along that look direction and see what we hit up to max range
	//HitLocation = FVector(1.0f);
	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector & HitLocation, FVector LookDirection) const
{
	FHitResult HitResult;
	FVector Start = PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (LookDirection * LineTraceRange);

	//UE_LOG(LogTemp, Warning, TEXT("Start: %s, End: %s"), *Start.ToString(), *End.ToString());
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.f);
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector & WorldDirection) const
{
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraLocation, 
		WorldDirection);

	// TODO Figure out why DeprojectScreenPositionToWorld return false even though WorldDirection is valid
	return true;
}