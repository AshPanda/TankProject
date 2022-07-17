// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Projectile.h"
#include "TankPawn.h"
#include "AmmoBox.h"
#include "Canon.generated.h"

class AAmmoBox;
UCLASS()
class TANKOGEDDON_API ACanon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanon();

	void Fire();
	void Reload();
	void FireSpecial();
	void Burst();

	void StopFire();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 Bullets = 3;

	UPROPERTY()
	ATankPawn* Tank;


protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		//float FireRate = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float FireRange = 3000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		float FireDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BurstSize = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float BurstInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ReloadTime = 1.0f;

	FTimerHandle ReloadTimer;
	FTimerHandle BurstTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	TSubclassOf<AProjectile> ProjectileClass;

	FHitResult hitResult;
public:	

private:
	bool bCanFire = true;
	int32 CurrrentBurst = 0;
	
};
