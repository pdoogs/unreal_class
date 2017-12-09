// Copyright Patrick Dugan

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	// TODO Clamp actual Throttle value so Player can't speed up tank
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}