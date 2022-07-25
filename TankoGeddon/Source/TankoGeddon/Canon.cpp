// Fill out your copyright notice in the Description page of Project Settings.


#include "Canon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "DamageTaker.h"
#include "DrawDebugHelpers.h"
#include "Pool.h"



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
	if (CannonType == ECannonType::FireProjectile || CannonType == ECannonType::FireProjectilePlazma  )
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile/Plazma")));
		if (ProjectilePool)
		{
			ProjectilePool->GetProjectile(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		}
		else
		{
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->OnKilled.AddUObject(this, &ACanon::AddScore);
				projectile->SetOwner(this);
				projectile->Start();
				
			}
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire trace")));
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 20);

			
			if (hitResult.GetActor())
			{
				
				AActor* owner = GetOwner();
				AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;
				if (hitResult.GetActor() != owner && hitResult.GetActor() != ownerByOwner)
				{
					FDamageData damageData;
				    damageData.DamageValue = FireDamage;
					damageData.Instigator = owner;
					damageData.DamageMaker = this;

					//Tank->GetActor();
					Tank->TakeDamage(damageData);//крашится при попадании
				}
				else
				{
					hitResult.GetActor()->Destroy();
				}

			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 0.5f, 0, 20);
		}
		
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, ReloadTime, false);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Number of bullets left: %d"), Bullets));
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
		return;
	}
	CurrrentBurst++;
	
	if (CannonType == ECannonType::FireProjectile  || CannonType == ECannonType::FireProjectilePlazma)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile/Plazma")));
		if (ProjectilePool)
		{
			ProjectilePool->GetProjectile(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		}
		else
		{
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->OnKilled.AddUObject(this, &ACanon::AddScore);
				projectile->SetOwner(this);
				projectile->Start();
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire trace")));
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 15);
			if (hitResult.GetActor())
			{
				hitResult.GetActor()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 0.5f, 0, 15);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Number of bullets left: %d"), Bullets));
	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACanon::Burst, BurstInterval, false);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACanon::Reload, ReloadTime, false);
}




void ACanon::StopFire()
{
	bCanFire = false;
}

void ACanon::AddScore(float ScoreValue)
{
	if (ScoreChanged.IsBound())
	{
		ScoreChanged.Broadcast(ScoreValue);
	}
}

void ACanon::CreateProjectilePool()
{
	if (ProjectilePoolClass)
		ProjectilePool = GetWorld()->SpawnActor<APool>(ProjectilePoolClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();
	CreateProjectilePool();
	
	
}
