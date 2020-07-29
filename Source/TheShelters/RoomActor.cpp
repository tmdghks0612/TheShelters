// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"

// Sets default values
ARoomActor::ARoomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("room actor constructing..."));

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
}

void ARoomActor::RoomMeshRandomize()
{
	if (RoomMeshVersions.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("room actor available!"));

		TArray<UStaticMeshComponent*> StaticComps;
		GetComponents<UStaticMeshComponent>(StaticComps);

		int version = rand() % RoomMeshVersions.Num();
		StaticComps[0]->SetStaticMesh(RoomMeshVersions[version]);

		/*
		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		RoomMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
		RoomMeshComponent->AttachTo(Root);

		int version = rand() % RoomMeshVersions.Num();
		RoomMeshComponent->SetStaticMesh(RoomMeshVersions[version]);

		ZapPlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZapPlane"));
		ZapPlaneComponent->AttachTo(Root);

		ZapPlaneComponent->SetStaticMesh(ZapPlaneMesh);*/
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("no available room version!"));
	}
}


// Called when the game starts or when spawned
void ARoomActor::BeginPlay()
{
	Super::BeginPlay();
	
}




