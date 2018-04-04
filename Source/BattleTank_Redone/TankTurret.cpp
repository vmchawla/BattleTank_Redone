// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::InitiateRotate(float DeltaRotatorYaw)
{
	auto RotationChange = FMath::Clamp(DeltaRotatorYaw, -1.0f, 1.0f) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewRotationYaw = RelativeRotation.Yaw + RotationChange;

	SetWorldRotation(FRotator(0.0f, NewRotationYaw, 0.0f));
}


