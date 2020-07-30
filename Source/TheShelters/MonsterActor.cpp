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
	MonsterSkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	if (monsterType == 0) {
		static ConstructorHelpers::FClassFinder<UAnimInstance> GHOUL_ANIM(TEXT("/Game/MonsterBP/GhoulAnimationBP.GhoulAnimationBP_C"));
		if (GHOUL_ANIM.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("GHOUL_ANIM Succeed"));
			MonsterSkeletalMeshComponent->SetAnimInstanceClass(GHOUL_ANIM.Class);
		}
	}
	
	
	// other monster types added here
}

void AMonsterActor::ChargePanicRoom()
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	UE_LOG(LogTemp, Warning, TEXT("monster is angry!"));
	chargeLocation = GetActorLocation();

	IsAngry = true;
	MonsterAnimInstance->SetAngry(true);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterActor::RestoreAngry, 5.0f, false);
	
	/*TimerDel.BindUFunction(this, FName("RestoreAngry"));
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.0f, false);*/
	return;
}

void AMonsterActor::RestoreAngry()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
	IsAngry = false;
	MonsterAnimInstance->SetAngry(false);
	StopCharge();
	SetActorLocation(chargeLocation);
	//MonsterSkeletalMeshComponent->SetAnimInstanceClass(AnimationBPs[0]->GetAnimBlueprintGeneratedClass());
}

void AMonsterActor::EnterPanicRoom()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster entered panic room!"));
	destination = GetActorLocation();
	return;
}

bool AMonsterActor::IsDoorOpen()
{
	if (!(roomControl->IsBlocked(monsterId))) {
		//UE_LOG(LogTemp, Warning, TEXT("panic room is blocked!"));
	}
	return !(roomControl->IsBlocked(monsterId));
}

void AMonsterActor::InitMonsterActor(ARoomControl* _roomControl, int _monsterId, int _monsterType)
{
	monsterId = _monsterId;
	monsterType = _monsterType;
	roomControl = _roomControl;
	return;
}

void AMonsterActor::MoveTo(FVector _destination)
{
	IsMoving = true;
	destination = _destination;
	chargeDirection = _destination - GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("move to %f %f"), chargeDirection.X, chargeDirection.Y);
	if (!IsAngry) {
		MonsterAnimInstance->SetMovement(true);
	}
}

void AMonsterActor::StopCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("stopped!!"));
	IsMoving = false;
	MonsterAnimInstance->SetMovement(false);
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
	Super::BeginPlay();
	
	destination = GetActorLocation();
	MonsterAnimInstance = Cast<UMonsterAnimInstance>(MonsterSkeletalMeshComponent->GetAnimInstance());

	if (MonsterAnimInstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("monster is null!"));
	}

}


