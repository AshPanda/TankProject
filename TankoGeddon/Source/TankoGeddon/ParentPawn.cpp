// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentPawn.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Components\BoxComponent.h"
#include "HealthComponent.h"
#include "Canon.h"

// Sets default values
AParentPawn::AParentPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AParentPawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AParentPawn::DamageTaked);
}

void AParentPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}



float AParentPawn::GetPoints()
{
	return ScoreValue;
}

void AParentPawn::ShowScore(float Value)
{
	Score += ScoreValue;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Silver, FString::Printf(TEXT("Score: %f"), Score));
}

// Called when the game starts or when spawned
void AParentPawn::BeginPlay()
{
	Super::BeginPlay();

	
}

void AParentPawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	
	Destroy();
	//ShowScore(Score);
	
}

void AParentPawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s take Damage: %f,  Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

