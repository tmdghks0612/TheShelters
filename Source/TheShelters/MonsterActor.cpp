// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterActor.h"

// Sets default values
AMonsterActor::AMonsterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!MonsterSkeletalMesh) {
		UE_LOG(LogTemp, Warning, TEXT("monster skeletal mesh exists!"));

		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		MonsterSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterSkeletalMesh"));
		MonsterSkeletalMeshComponent->AttachTo(Root);

		MonsterSkeletalMeshComponent->SetSkeletalMesh(MonsterSkeletalMesh, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("monster skeletal mesh absent"));
	}
	
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
	Super::BeginPlay();
	
}


