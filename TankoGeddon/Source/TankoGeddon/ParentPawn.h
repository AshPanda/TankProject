// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "IScorable.h"
#include "HealthComponent.h"//
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParentPawn.generated.h"

class UStaticMeshComponent;
class ACanon;
UCLASS()
class TANKOGEDDON_API AParentPawn : public APawn, public IDamageTaker, public IIScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AParentPawn();

	virtual void TakeDamage(FDamageData DamageData) override;

	virtual float GetPoints() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scorable")
	float ScoreValue = 5.5f;

	UFUNCTION()
	void ShowScore(float Value);

	float Score = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UParticleSystemComponent* ShootEffectKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UAudioComponent* AudioEffectKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UParticleSystemComponent* ShootEffectDamaged;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Effects")
	UAudioComponent* AudioEffectDamaged;
	UPROPERTY()
	float DeathTime = 5.0f;
	FTimerHandle DeathTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY()
		ACanon* Cannon;

	UFUNCTION()
		void Die();
	UFUNCTION()
		void DeathDelay();
	UFUNCTION()
		void DamageTaked(float DamageValue);

private:
	bool bCanDie = true;
};
