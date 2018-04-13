// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
//#include "Public/DrawDebugHelpers.h"
#include "TankAimingComponent.h"
#include "Tank.h"



void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceAndPassHitLocationToTank();

}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank))
		{
			return;
		}
		//Subscribe our local method to the tanks death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Tank Player Controller has heard your broadcast of death"));
	//StartSpectatingOnly();
	StartSpectatingOnly();
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
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, StartLocation + (LookDirection * LineTraceRange), ECollisionChannel::ECC_Camera))
		{
			auto HitLocation = HitResult.Location;
			//UE_LOG(LogTemp, Warning, TEXT("HitLocation from LineTrace: %s"), *(HitLocation.ToString()));
			if (GetPawn())
			{
				GetPawn()->FindComponentByClass<UTankAimingComponent>()->AimAt(HitLocation);
			}
		}
	}

}






