// Copyright Patrick Dugan

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float TimeSeconds) override;

	// How close can the AI tank get to the player
	float AcceptanceRadius = 3000;
};
