// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"

#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent * sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;
	
	GatesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
	GatesMesh->SetupAttachment(sceneComponent);
	
	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
	ActivatedLight->SetupAttachment(sceneComponent);
	
	DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
	DeactivatedLight->SetupAttachment(sceneComponent);
	
	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
	TriggerCollider->SetupAttachment(GatesMesh);
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);
	
	//SetActiveLights();

}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
	IsActivated = NewIsActivated;
	SetActiveLights();
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	SetActiveLights();
	
}

void AMapLoader::SetActiveLights()
{
	ActivatedLight->SetHiddenInGame(!IsActivated);
	DeactivatedLight->SetHiddenInGame(IsActivated);
}

void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsActivated)
	{
		return;
	}
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(),LoadLevelName);
	}
}

