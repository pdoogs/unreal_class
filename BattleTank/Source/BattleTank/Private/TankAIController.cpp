// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

ATank * ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController is controlling %s"), *(ControlledTank->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController not controlling any tank"));
	}

	UE_LOG(LogTemp, Warning, TEXT("TankAIController Begin Play"));
}
