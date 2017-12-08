// Fill out your copyright notice in the Description page of Project Settings.

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
		auto ControlledTank = Cast<ATank>(GetPawn());

		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		ControlledTank->Fire(); // Don't Fire every frame
	}
}