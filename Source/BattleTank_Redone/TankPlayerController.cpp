// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"



void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceAndPassHitLocationToTank();

}

void ATankPlayerController::LineTraceAndPassHitLocationToTank() 
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	float CrosshairViewPortLocationX = float(ViewportSizeX) * CrossHairXLocation;
	float CrosshairViewPortLocationY = float(ViewportSizeY) * CrossHairYLocation;

	auto ScreenLocation = FVector2D(CrosshairViewPortLocationX, CrosshairViewPortLocationY);

	bool isDeprojectScreenToWorldSuccess;

	FVector CameraWorldLocation; //Out Parameter Discarded
	FVector LookDirection; //Out Parameter Used

	isDeprojectScreenToWorldSuccess = DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);


	if (isDeprojectScreenToWorldSuccess)
	{
		auto StartLocation = PlayerCameraManager->GetCameraLocation();
		//DrawDebugLine(GetWorld(), StartLocation, StartLocation + (LookDirection * LineTraceRange), FColor::Red, false, 0.0f, 0.0f, 10.0f);
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, StartLocation + (LookDirection * LineTraceRange), ECollisionChannel::ECC_Visibility))
		{
			auto HitLocation = HitResult.Location;
			//UE_LOG(LogTemp, Warning, TEXT("HitLocation from LineTrace: %s"), *(HitLocation.ToString()));
			if (Cast<ATank>(GetPawn()))
			{
				Cast<ATank>(GetPawn())->AimAt(HitLocation);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TankPlayerController.cpp has failed to GetPawn() and pass hit location to player controlled tank"));
			}
			
		}
	}

}




