// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components\StaticMeshComponent.h"
#include "Components\BoxComponent.h"
#include "TimerManager.h"
#include "Components\ArrowComponent.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet\GameplayStatics.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent * sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComponent);

	RubbleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rubble Mesh"));
	RubbleMesh->SetupAttachment(sceneComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);

	ShootEffectFactoryKilled = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect On Factory Distruction"));
	ShootEffectFactoryKilled->SetupAttachment(RubbleMesh);

	ShootEffectTankSpawned = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect On New Tank Spawned"));
	ShootEffectTankSpawned->SetupAttachment(BuildingMesh);

}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	if(LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(false);
	}
	BuildingMesh->SetHiddenInGame(bIsActivated);
	
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
	RubbleMesh->SetHiddenInGame(!bIsActivated);
	
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	newTank->SetPatrollingPoints(TankWayPoints);
	if (!ShootEffectTankSpawned)
	{
		return;
	}
	else
	{
		ShootEffectTankSpawned->ActivateSystem();
	}
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
}

void ATankFactory::Die()
{
	if(LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(true);
	}
	if (!ShootEffectFactoryKilled)
	{
		return;
	}
	else
	{
		ShootEffectFactoryKilled->ActivateSystem();
	}
	SetActiveRubble();
	GetWorld()->GetTimerManager().ClearTimer(_targetingTimerHandle);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::SetIsActivated(bool NewIsActivated)
{
	bIsActivated = NewIsActivated;
	SetActiveRubble();
}

void ATankFactory::SetActiveRubble()
{
	RubbleMesh->SetHiddenInGame(bIsActivated);
	BuildingMesh->SetHiddenInGame(!bIsActivated);
}
