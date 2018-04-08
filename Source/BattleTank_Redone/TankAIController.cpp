// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
//Depends on movemement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto AIControlledPawn = GetPawn();

	if (ensure(PlayerPawn && AIControlledPawn))
	{
		//move Towards Player
		//TODO check radius is in CM
		MoveToActor(PlayerPawn, AcceptanceRadius);
		auto AimingComponent = AIControlledPawn->FindComponentByClass<UTankAimingComponent>();
		AimingComponent->AimAt(PlayerPawn->GetActorLocation());
		AimingComponent->Fire(); 
	}
}




