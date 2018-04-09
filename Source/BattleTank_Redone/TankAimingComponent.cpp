// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


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

EFiringStatus UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	LastFireTime = GetWorld()->GetTimeSeconds();
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("Aim Direction Nomralized: %s"), *(AimDirectionNormalized.ToString()));

	bool isReloaded = (GetWorld()->GetTimeSeconds() - LastFireTime) > ReloadTimeInSeconds;
	bool hasAmmo = Ammo > 0;

	//if (isReloaded)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Reloaded"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("NOT RELOADED"))
	//}
	//
	//if (IsBarrelMoving())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Barrel is moving"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Barrel is NOT moving"));
	//}

	if (!isReloaded && hasAmmo)
	{
		FiringState = EFiringStatus::Reloading;
	}
	 else if (hasAmmo && isReloaded && IsBarrelMoving())
	{
		FiringState = EFiringStatus::Aiming;
	}
	else if (hasAmmo && isReloaded && !IsBarrelMoving())
	{
		FiringState = EFiringStatus::Locked;
	}
	else if (!hasAmmo)
	{
		FiringState = EFiringStatus::NoAmmo;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel))
	{
		return false;
	}

	auto BarrelForwardVector = Barrel->GetForwardVector();
	
	return !BarrelForwardVector.Equals(AimDirectionNormalized, 0.01f);
}

void UTankAimingComponent::AimAt(FVector HitLocation)
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
			AimDirectionNormalized = OutLaunchVelocity.GetSafeNormal();
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

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return;  }
	

	if (FiringState != EFiringStatus::Reloading && FiringState != EFiringStatus::NoAmmo)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
		Ammo = Ammo - 1;
	}
}








