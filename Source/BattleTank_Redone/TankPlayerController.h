// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_REDONE_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		float CrossHairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
		float CrossHairYLocation = 0.33333f;

	UPROPERTY(EditDefaultsOnly)
		float LineTraceRange = 1000000;

	void LineTraceAndPassHitLocationToTank();

	UFUNCTION()
		void OnPossessedTankDeath();

public:
	//Called Every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void SetPawn(APawn* InPawn) override;


	
};
