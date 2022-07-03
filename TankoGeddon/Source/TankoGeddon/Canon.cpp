// Fill out your copyright notice in the Description page of Project Settings.


#include "Canon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanonMesh"));
	RootComponent = CannonMesh;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACanon::Fire()
{
	if (!bCanFire)
	{
		return;
	}

	bCanFire = false;

	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire trace")));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, ReloadTime, false);
}

void ACanon::Reload()
{
	bCanFire = true;
}

// Called when the game starts or when spawned
/*void ACanon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
