// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h" //So we can implement on death
//Depends on movemement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn * InPawn)
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
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto AIControlledPawn = GetPawn();

	if (!PlayerPawn) { return; }

	if (!ensure(AIControlledPawn)) { return; }

	if (ensure(PlayerPawn && AIControlledPawn))
	{
		//move Towards Player
		//TODO check radius is in CM
		MoveToActor(PlayerPawn, AcceptanceRadius);
		auto AimingComponent = AIControlledPawn->FindComponentByClass<UTankAimingComponent>();
		if (!(AimingComponent)) 
		{ 
			UE_LOG(LogTemp, Warning, TEXT("NO Aiming Component found on AI Controller. Returning"));
			return;  
		}
		AimingComponent->AimAt(PlayerPawn->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
		{
			AimingComponent->Fire();
		}
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Tank AI Controller has heard your broadcast od Death"));
	//PossessedTank->DetachFromControllerPendingDestroy();
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}




