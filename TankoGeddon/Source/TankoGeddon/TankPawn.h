// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStruct.h"
#include "DamageTaker.h"
#include "ParentPawn.h"
#include "Engine/TargetPoint.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACanon;
class ANewAmmo;
class AAmmoBox;
UCLASS()
class TANKOGEDDON_API ATankPawn : public AParentPawn/*, public IDamageTaker, public IIScorable*/
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void RotateRight(float Value);

	void Fire();

	void FireSpecial();

	void AddBullets();

	void WeaponChange();
	
;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Components")
	int32 ammoPool = 0;


	UPROPERTY()
	AAmmoBox* ammo;

	//UPROPERTY()
	//ACanon* Cannon;
	UPROPERTY()
	class APawn* PlayerPawn;

	UFUNCTION()
	float GetAccurency() { return MovementAccurency; }

	UFUNCTION()
    TArray<FVector> GetPatrollingPoints();

	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);

	UFUNCTION()
	FVector GetTurretForwardVector();

	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition();

protected:
	// Called when the game starts or when spawned
	

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UStaticMeshComponent* BodyMesh;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Components")
	TSubclassOf<ACanon> EquippedCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Components")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Component")
	TSubclassOf<ACanon> SecondCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotateSpeed = 55.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotateInterpolationKey = 0.1f;

	UPROPERTY()
	class ATankController* TankController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points" , Meta = (MakeEditWidget = true))
    TArray<ATargetPoint*> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI |Moveparams|Accurency")
	float MovementAccurency = 30.0f;

	


public:	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupCannon(TSubclassOf<ACanon> newCannonClass);



private:
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;
	float RotateRightAxisValue = 0.0f;
	float CurrentRotateAxisValue = 0.0f;
};

