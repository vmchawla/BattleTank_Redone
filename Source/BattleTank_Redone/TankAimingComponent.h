// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM(BlueprintType)
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked,
	NoAmmo
};


//Forward Declarations
class UTankBarrel;
class UTankTurret;
class AProjectile;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_REDONE_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
		void Fire();

	EFiringStatus GetFiringState() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		EFiringStatus FiringState = EFiringStatus::Reloading;

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int32 Ammo = 3;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 20000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float ReloadTimeInSeconds = 3.0f;

	//Passing Pitch and Yaw Values to Barrel and Turret Respectively
	void MoveBarrelAndTurretTowards(FVector AimDirectionNormalized);

	double LastFireTime = 0;

	bool IsBarrelMoving();

	FVector AimDirectionNormalized;


		
	
};
