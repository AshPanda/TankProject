// Fill out your copyright notice in the Description page of Project Settings.



#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Canon.h"
#include "AmmoBox.h"
#include <Components/SceneComponent.h>
#include <Engine/EngineTypes.h>
#include <Components/ArrowComponent.h>

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

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
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACanon>(newCannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
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

	FVector MousePos = TankController->GetMousePosition();

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation,targetRotation, RotateInterpolationKey));

}
// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f));

	TankController = Cast<ATankController>(GetController());

	SetupCannon(CannonClass);
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

