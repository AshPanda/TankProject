// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(AProjectile, FOnKill, float);
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION()
	virtual void Start();

	void Deactivate();

	FOnKill OnKilled;

	bool bIsActivation = false;
	virtual void Explode();
	
	bool bIsExplodeActivated = false;
	
	void TakeDamageAndMovement(class AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float MoveSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float MoveRate = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Damage = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float DeactivateTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float PushForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	float ExplodeRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	int32 TrajectoryPointIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	float MovementAccurency = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UPhysicsComponent* PhysicsComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	float MaxTimeSimulation = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	float TimeStep = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params | Trajectory")
	float MoveSpeedPhysics = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement params")
	FVector MoveVector;

	FTimerHandle MoveTimer;
	FTimerHandle DeactivateTimer;
	// Called when the game starts or when spawned
	UFUNCTION()
	virtual void Move();

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; */

};
