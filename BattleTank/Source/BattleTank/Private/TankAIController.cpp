// Copyright Patrick Dugan

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!GetPawn()) return;

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!ensure(PlayerTank && ControlledTank)) { return; }

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	auto PlayerTankAsTank = Cast<ATank>(PlayerTank);
	if (ensure(PlayerTankAsTank)) {
		PlayerTankAsTank->Fire(); // Don't Fire every frame
	}
}