// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet\KismetMathLibrary.h"
#include "DamageTaker.h"
#include "GameStruct.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()

{
	//Super::Start();
	MoveVector = GetActorForwardVector() * MoveSpeedPhysics; ///
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, MaxTimeSimulation, TimeStep, 0.0f);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Start")));
	if (bShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5, 8, FColor::Purple, true, 1, 0, 2);
			
		}
	}
	
	TrajectoryPointIndex = 0;
	TrailEffect->ActivateSystem();
	
}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();

	//UE_LOG(LogTemp, Warning, TEXT("Not normalize: %s"), *CurrentMoveVector.ToString());
	CurrentMoveVector.Normalize();
	//UE_LOG(LogTemp, Warning, TEXT("Normalize: %s"), *CurrentMoveVector.ToString());

	FVector newLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);

	if (FVector::Distance(newLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MovementAccurency)
	{
		TrajectoryPointIndex++;
		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
	//Super::Move();
}

void APhysicsProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";

	TArray<FHitResult> AttachHit;

	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel(AttachHit, startPos, endPos, Rotation, ECollisionChannel::ECC_Visibility, Shape, params);

	DrawDebugSphere(GetWorld(), startPos, ExplodeRadius, 5, FColor::Green, false, 2.0f);

	if (bSweepResult)
	{
		for (FHitResult hitResult : AttachHit)
		{
			AActor* OtherActor = hitResult.GetActor();
			if(!OtherActor)
				continue;

			IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
			if (DamageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(damageData);
			}
			else
			{
				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
				if (mesh)
				{
					if (mesh->IsSimulatingPhysics())
					{
						FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						mesh->AddForce(forceVector * PushForce, NAME_None, true);
					}
				}
			}
		}
	}
}
