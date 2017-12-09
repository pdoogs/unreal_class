// Copyright Patrick Dugan

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//ATank * PlayerTank = GetPlayerTank();
	ATank * PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerTank)
	{
		MoveToActor(
			PlayerTank,
			AcceptanceRadius); // TODO check radius is in cm

		//UE_LOG(LogTemp, Warning, TEXT("MovingToActor %s"), *PlayerTank->GetName());

		auto ControlledTank = Cast<ATank>(GetPawn());

		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		ControlledTank->Fire(); // Don't Fire every frame
	}
}