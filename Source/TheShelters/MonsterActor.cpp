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
	UE_LOG(LogTemp, Warning, TEXT("saving on %f %f"), chargeLocation.X, chargeLocation.Y);
	IsAngry = true;
	MonsterAnimInstance->SetAngry(true);
	IsMoving = false;
	MonsterAnimInstance->SetMovement(false);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterActor::RestoreAngry, 5.0f, false);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonsterActor::ActiveAngry, ChargeDelay, false);
	/*TimerDel.BindUFunction(this, FName("RestoreAngry"));
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.0f, false);*/
	return;
}

void AMonsterActor::RestoreAngry()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
	IsAngry = false;
	IsCharge = false;
	MonsterAnimInstance->SetAngry(false);
	StopCharge();
	SetActorLocation(chargeLocation);
	UE_LOG(LogTemp, Warning, TEXT("restore to %f %f"), chargeLocation.X, chargeLocation.Y);
	//MonsterSkeletalMeshComponent->SetAnimInstanceClass(AnimationBPs[0]->GetAnimBlueprintGeneratedClass());
}

void AMonsterActor::ActiveAngry()
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	IsCharge = true;
	MonsterAnimInstance->SetAngry(false);
	MonsterAnimInstance->SetMovement(true);

}

void AMonsterActor::EnterPanicRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("monster entered panic room! Game Over"));
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

int AMonsterActor::GetMonsterId()
{
	return monsterId;
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
	IsCharge = false;
	MonsterAnimInstance->SetMovement(false);

	if (roomControl->CheckPanicRoom(monsterId)) {
		ChargePanicRoom();
	}
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


