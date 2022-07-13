// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAmmo.h"
#include "Canon.h"
#include "TankPawn.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ANewAmmo::ANewAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* NewAmmoSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = NewAmmoSceneComponent;

	NewAmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NewAmmoMesh"));
	NewAmmoMesh->SetupAttachment(RootComponent);
	NewAmmoMesh->OnComponentBeginOverlap.AddDynamic(this, &ANewAmmo::OnMeshOverlapBegin);
	NewAmmoMesh->SetCollisionProfileName(FName("OverlapAll"));
	NewAmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	NewAmmoMesh->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
/*void ANewAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}*/

void ANewAmmo::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);


	if (TankPawn)
	{
		TankPawn->AddMoreBullets(OtherActor);
		TankPawn->SetupCannon(CannonClass);
		this->Destroy();
	}

}

// Called every frame
/*void ANewAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/
