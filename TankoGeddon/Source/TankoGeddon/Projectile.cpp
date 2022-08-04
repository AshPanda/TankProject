// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "ParentPawn.h"
#include "IScorable.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);
	GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &AProjectile::Deactivate, DeactivateTime, false);
}

void AProjectile::Deactivate()
{
	bIsActivation = false;
	SetActorLocation(FVector(0.0f, 0.0f, -150.0f));
	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimer);
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	SetActorEnableCollision(false);
}

void AProjectile::Explode()
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

void AProjectile::TakeDamageAndMovement(class AActor* OtherActor)
{
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);
		IIScorable* ScorableActor = Cast<IIScorable>(OtherActor);
		
		float ScoreValue = 0.0f;

		if (ScorableActor)
		{
			ScoreValue = ScorableActor->GetPoints();
			
		}

		if (damageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			damageTakerActor->TakeDamage(damageData);

			if (OtherActor->IsActorBeingDestroyed() && ScoreValue != 0.0f)
			{
				if (OnKilled.IsBound())
				{
					OnKilled.Broadcast(ScoreValue);
					
				}
				
			}
			
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
					if (bIsExplodeActivated)
					{
						Explode();
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Mesh is not vallid"));
			}
			
		}

		Deactivate();
		
	}
	
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
	if (bIsExplodeActivated)
	{
		Explode();
	}
}

void AProjectile::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);
		IIScorable* ScorableActor = Cast<IIScorable>(OtherActor);
		
		float ScoreValue = 0.0f;

		if (ScorableActor)
		{
			ScoreValue = ScorableActor->GetPoints();
			
		}

		if (damageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			damageTakerActor->TakeDamage(damageData);

			if (OtherActor->IsActorBeingDestroyed() && ScoreValue != 0.0f)
			{
				if (OnKilled.IsBound())
				{
					OnKilled.Broadcast(ScoreValue);
					
				}
				
			}
			
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
					if (bIsExplodeActivated)
					{
						Explode();
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Mesh is not vallid"));
			}
			
		}

		Deactivate();
		
	}
	
}

// Called when the game starts or when spawned
/*void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

*/

