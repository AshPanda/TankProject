// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pool.generated.h"

class AProjectile;
UCLASS()
class TANKOGEDDON_API APool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	void GetProjectile(FVector spawnLocation, FRotator spawnRotation);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation, bool bIsActive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	int32 PoolSize = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	TSubclassOf<AProjectile> ProjectileClass;

	TArray<AProjectile*> ProjectilePool;

	void InitializePool();

	bool bHaveActiveProjectile = false;
};
