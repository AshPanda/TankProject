// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Canon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBox();
	//void AddMoreBullets();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BulletsBox = 20;

	UFUNCTION()
	int32 getInt();



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* AmmoBoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class ACanon> CannonClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//TSubclassOf<class ACanon> Bullets;

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
