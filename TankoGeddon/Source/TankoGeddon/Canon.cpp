// Fill out your copyright notice in the Description page of Project Settings.


#include "Canon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* CannonSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CannonRoot"));
	RootComponent = CannonSceneComponent;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanonMesh"));
	CannonMesh->SetupAttachment(CannonSceneComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonSceneComponent);
}

void ACanon::Fire()
{
	if (!bCanFire || Bullets == 0)
	{
		return;
	}

	bCanFire = false;
	Bullets--;
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire trace")));
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Number of bullets left: %d"), Bullets));
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, ReloadTime, false);
}



void ACanon::FireSpecial()
{
	
	if (!bCanFire || Bullets == 0)
	{
		return;
	}
	bCanFire = false;
	Burst();
}
void ACanon::Reload()
{
	bCanFire = true;
}
void ACanon::Burst()
{
	if (CurrrentBurst == BurstSize)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		bCanFire = true;
		CurrrentBurst = 0;
		Bullets--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Number of bullets left: %d"), Bullets));
		return;
	}
	CurrrentBurst++;
	
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire trace")));
	}
	
	
	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACanon::Burst, BurstInterval, false);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, ReloadTime, false);
}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();
	//Reload();
	
}
/*
// Called every frame
void ACanon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
