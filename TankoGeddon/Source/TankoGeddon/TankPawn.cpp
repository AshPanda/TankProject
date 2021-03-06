// Fill out your copyright notice in the Description page of Project Settings.



#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Canon.h"
#include <Components/SceneComponent.h>
#include <Engine/EngineTypes.h>
#include <Components/ArrowComponent.h>
#include "HealthComponent.h"
#include "Engine\TargetPoint.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void ATankPawn::MoveForward(float Value)
{
	TargetForwardAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	TargetRightAxisValue = Value;
}


void ATankPawn::SetupCannon(TSubclassOf<ACanon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	EquippedCannonClass = newCannonClass;
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACanon>(EquippedCannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator CurrentTurretRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = CurrentTurretRotation.Pitch;
	targetRotation.Roll = CurrentTurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentTurretRotation, targetRotation, RotateInterpolationKey));
}


FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CurrentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = CurrentLocation + (forwardVector * MoveSpeed * TargetForwardAxisValue) + (rightVector * MoveSpeed * TargetRightAxisValue);
	SetActorLocation(movePosition, true);

	CurrentRotateAxisValue = FMath::Lerp(CurrentRotateAxisValue, RotateRightAxisValue, InterpolationKey);
	float YawRotation = RotateSpeed * CurrentRotateAxisValue * DeltaSeconds;
	FRotator CurrentRotation = GetActorRotation();
	
	//UE_LOG(LogTemp, Warning, TEXT("CurrentRotateAxis Value: %f, RotateRightAxisValue: %f"), CurrentRotateAxisValue, RotateRightAxisValue);
	YawRotation += CurrentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, YawRotation, 0.0f);
	SetActorRotation(newRotation);

	if (TankController)
	{
		FVector MousePos = TankController->GetMousePosition();
		RotateTurretTo(MousePos);
	}



}
// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	TankController = Cast<ATankController>(GetController());
	SetupCannon(EquippedCannonClass);
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::Fire()
{
	
	if (Cannon)
	{
		
		//Cannon->Fire();
		if (ammoPool > 0)
		{
			Cannon->Fire();
			ammoPool--;
		}
		else
		{
			Cannon->StopFire();
		}
		ShowScore(Score);

	}

}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		//Cannon->FireSpecial();
		if (ammoPool > 0)
		{
			Cannon->FireSpecial();
			ammoPool--;
		}
		else
		{
			Cannon->StopFire();
		}
		ShowScore(Score);
	}
	
}



void ATankPawn::AddBullets()
{           
	if (Cannon)
	{
		Cannon->Bullets = ammoPool;
		//UE_LOG(LogTemp, Warning, TEXT("Ammo in TankPawn ADD_Bullets Canon->Bullets: %d"), Cannon->Bullets);

	}
}


void ATankPawn::WeaponChange()
{
	
	TSubclassOf<ACanon> tempCannon = EquippedCannonClass;
	EquippedCannonClass = SecondCannonClass;
	SecondCannonClass = tempCannon;

	
	SetupCannon(EquippedCannonClass);
}

// Called to bind functionality to input
// void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);

//}

