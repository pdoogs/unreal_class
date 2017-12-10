// Copyright Patrick Dugan

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const & DamageEvent,
	class AController * EventInstigator,
	AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto DamageToApply = FMath::Clamp(DamageAmount, 0.0f, CurrentHealth);
	CurrentHealth -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("DamageToApply %f, DamageAmount %f"), DamageToApply, DamageAmount);

	return DamageToApply;
}