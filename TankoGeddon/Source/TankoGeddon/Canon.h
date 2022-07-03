// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Canon.generated.h"

UCLASS()
class TANKOGEDDON_API ACanon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanon();

	void Fire();
	void Reload();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ReloadTime = 1.0f;
	FTimerHandle ReloadTimer;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	bool bCanFire = true;
};
