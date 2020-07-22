// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotActor.h"

// Sets default values
ARobotActor::ARobotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RobotSkeletalMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Robot SkeletalMesh Assigned"));

		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		RobotSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotSkeletalMesh"));
		RobotSkeletalMeshComponent->AttachTo(Root);

		RobotSkeletalMeshComponent->SetSkeletalMesh(RobotSkeletalMesh, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Assigned RobotSkeletalMesh"));
	}

}



// Called when the game starts or when spawned
void ARobotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ARobotActor::CheckWorking()
{
	UE_LOG(LogTemp, Warning, TEXT("It's on"));
	return 1;
}

