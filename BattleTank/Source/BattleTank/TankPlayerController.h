// Copyright Patrick Dugan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * Responsible for helping the player aim.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

private:
	UFUNCTION()
	void OnTankDeath();

	virtual void SetPawn(APawn* InPawn) override;

	// Start the tank moving the barrel toward where a shot from the crosshair intersects the world
	void AimTowardCrosshair();

	bool GetSightRayHitLocation(FVector & HitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector & WorldDirection) const;

	bool GetLookVectorHitLocation(FVector & HitLocation, FVector LookDirection) const;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.333333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;
};
