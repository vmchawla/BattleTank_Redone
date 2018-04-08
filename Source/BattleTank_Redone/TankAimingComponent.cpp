// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}





void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//UE_LOG(LogTemp, Warning, TEXT("Barrel: %s and Turret: %s"), *(Barrel->GetName()), *(Turret->GetName()))
	
}




// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (Barrel && Turret)
	{
		FVector OutLaunchVelocity; //Out parameter
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

		bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity
		(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,
			0.0f,
			0.0f,
			ESuggestProjVelocityTraceOption::DoNotTrace,
			FCollisionResponseParams::DefaultResponseParam
		);

		if (bHasAimSolution)
		{
			auto AimDirectionNormalized = OutLaunchVelocity.GetSafeNormal();
			//UE_LOG(LogTemp, Warning, TEXT("Aim Direction Nomralized: %s"), *(AimDirectionNormalized.ToString()));
			MoveBarrelAndTurretTowards(AimDirectionNormalized);

		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Either Barrel or Turret or both not found in Tank Aiming Component. This should not happen. Check Reference passed from Tank class"));
	}
}

void UTankAimingComponent::MoveBarrelAndTurretTowards(FVector AimDirectionNormalized)
{
	if (!Barrel || !Turret)	{ return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirectionNormalized.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//UE_LOG(LogTemp, Warning, TEXT("Aim Rotator: %s"), *(AimAsRotator.ToString()));
	//UE_LOG(LogTemp, Warning, TEXT("Barrel Rotator: %s"), *(BarrelRotator.ToString()));
	//UE_LOG(LogTemp, Warning, TEXT("Delta Rotator: %s"), *(DeltaRotator.ToString()));

	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180.0f)
	{
		Turret->InitiateRotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->InitiateRotate(-DeltaRotator.Yaw);
	}
		
	
	

}






