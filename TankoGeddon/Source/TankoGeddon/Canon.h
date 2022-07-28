// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Projectile.h"
#include "TankPawn.h"
#include "DamageTaker.h"
#include "AmmoBox.h"
#include "Turret.h"
#include "Camera\CameraShakeBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Canon.generated.h"

class APool;
class AAmmoBox;
UCLASS()
class TANKOGEDDON_API ACanon : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(ACanon, FChangeScore, float);
	DECLARE_EVENT(ACanon, FOnKill);
	
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
	ATurret* Turret;
	UPROPERTY()
	ATankPawn* Tank;

	bool IsReadyToFire() { return bCanFire; };

	float Score = 0.0f;

	UFUNCTION()
	void AddScore(float ScoreValue);

	FChangeScore ScoreChanged;

	void CreateProjectilePool();

	FOnKill OnKilled;

	//UPROPERTY()
	IDamageTaker* damageTaker;
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<APool> ProjectilePoolClass;

	UPROPERTY()
	APool* ProjectilePool;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	TSubclassOf<class UCameraShakeBase> CameraShakeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect * ShootForceEffect;

private:
	bool bCanFire = true;
	int32 CurrrentBurst = 0;
	
};
