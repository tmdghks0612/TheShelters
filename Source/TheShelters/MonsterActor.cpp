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

void AMonsterActor::ChargePanicRoom()
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	UE_LOG(LogTemp, Warning, TEXT("monster is angry!"));
	IsAngry = true;
	FVector currentLocation = GetActorLocation();
	chargeDirection = panicRoomLocation - currentLocation;
	MonsterSkeletalMeshComponent->SetAnimInstanceClass(AnimationBPs[1]->GetAnimBlueprintGeneratedClass());

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterActor::RestoreAngry, 5.0f, false);
	/*TimerDel.BindUFunction(this, FName("RestoreAngry"));
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.0f, false);*/
	return;
}

void AMonsterActor::RestoreAngry()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
	IsAngry = false;
	MonsterSkeletalMeshComponent->SetAnimInstanceClass(AnimationBPs[0]->GetAnimBlueprintGeneratedClass());
}

void AMonsterActor::EnterPanicRoom()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster entered panic room!"));
	panicRoomLocation = GetActorLocation();
	return;
}

bool AMonsterActor::IsDoorOpen()
{
	if (!(roomControl->IsBlocked(monsterId))) {
		//UE_LOG(LogTemp, Warning, TEXT("panic room is blocked!"));
	}
	return !(roomControl->IsBlocked(monsterId));
}

void AMonsterActor::InitMonsterActor(ARoomControl* _roomControl, int _monsterId)
{
	monsterId = _monsterId;
	roomControl = _roomControl;
	return;
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
	Super::BeginPlay();
	
	panicRoomLocation = FVector(9000.0f, 0.0f, 200.0f);
}


