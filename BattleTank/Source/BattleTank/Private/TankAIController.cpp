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
	if (!ensure(PlayerTank)) { return; }

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// if Aiming or Locked only
	if (AimingComponent->GetFiringState() == EFiringState::Aiming ||
		AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (!PossessedTank) { return; }
	
		UE_LOG(LogTemp, Warning, TEXT("--- AI Controller Registered OnTankDeath"));

		// TODO Subscribe our local method to Tank's Death Event
		PossessedTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::OnTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("--- AI Tank Died!!!!!"));
	if (ensure(GetPawn()))
	{
		GetPawn()->DetachFromControllerPendingDestroy();
	}
}