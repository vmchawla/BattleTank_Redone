// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"


ATank * ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn not found by Tank AI Controller"));
		return nullptr;
	}
	return Cast<ATank>(PlayerPawn);
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerTank())
	{
		auto PlayerTankLocation = GetPlayerTank()->GetActorLocation();
		GetControlledTank()->AimAt(PlayerTankLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *(PlayerTankLocation.ToString()));
	}
}




