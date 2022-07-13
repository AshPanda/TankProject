// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewAmmo.generated.h"

UCLASS()
class TANKOGEDDON_API ANewAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewAmmo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BulletsBox = 20;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* NewAmmoMesh;

	UFUNCTION()
		void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
