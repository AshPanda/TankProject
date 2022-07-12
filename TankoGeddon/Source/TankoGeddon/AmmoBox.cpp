// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "Components/PrimitiveComponent.h"
#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* AmmoSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = AmmoSceneComponent;

	AmmoBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoBoxMesh"));
	AmmoBoxMesh->SetupAttachment(RootComponent);
	AmmoBoxMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	AmmoBoxMesh->SetCollisionProfileName(FName("OverlapAll"));
	AmmoBoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AmmoBoxMesh->SetGenerateOverlapEvents(true);

}

int32 AAmmoBox::getInt()
{
	return BulletsBox;
}



void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);

	ACanon* Bullets = Cast<ACanon>(OtherActor);

	if (TankPawn)
	{
		TankPawn->SetupCannon(CannonClass);
		Destroy();
	}

	if (Bullets)
	{
		Bullets->Bullets = Bullets->Bullets + BulletsBox;
		this->Destroy();
	}
	
}


