// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void BeginPlay() override;

	void RotateRight(float Value);
	

protected:
	// Called when the game starts or when spawned
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TargetForwardAxisValue = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float TargetRightAxisValue = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
