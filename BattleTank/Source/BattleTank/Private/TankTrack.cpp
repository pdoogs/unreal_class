// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"




void UTankTrack::SetThrottle(float Throttle)
{
	// TODO Clamp actual Throttle value so Player can't speed up tank
	UE_LOG(LogTemp, Warning, TEXT("%s SetThrottle: %f"), *(GetOwner()->GetName()), Throttle);
}