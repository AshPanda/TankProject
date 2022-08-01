// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "MapLoader.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();
	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SpawnNewTank();
	
	UFUNCTION()
	void Die();
	
	UFUNCTION()
	void DamageTaked(float DamageValue);

	void SetIsActivated(bool NewIsActivated);
	void SetActiveRubble();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RubbleMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * TankSpawnPoint;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * HitCollider;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent * HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	AMapLoader* LinkedMapLoader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UParticleSystemComponent* ShootEffectFactoryKilled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UParticleSystemComponent* ShootEffectTankSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory Visibility params")
	bool bIsActivated = false;

	FTimerHandle _targetingTimerHandle;
};
