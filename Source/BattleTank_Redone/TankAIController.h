// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"


/**
 * 
 */
UCLASS()
class BATTLETANK_REDONE_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 5000.0f;

	UFUNCTION()
		void OnPossessedTankDeath();
	
	
};
