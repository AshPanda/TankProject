// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();

	virtual void Start() override;

protected:
	virtual void Move() override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//class UPhysicsComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* TrailEffect;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	//float MovementAccurency = 10.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	//float MaxTimeSimulation = 50.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	//float TimeStep = 1.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	//float MoveSpeedPhysics = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	bool bShowTrajectory = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	//float ExplodeRadius = 50.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	//FVector MoveVector;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	//TArray<FVector> CurrentTrajectory;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	//int32 TrajectoryPointIndex;

	virtual void Explode() override;
};
