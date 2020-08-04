// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "RoomControl.h"

// Sets default values
AMonster::AMonster()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    if (!MonsterSkeletalMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("monster skeletal mesh exists!"));

        Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
        RootComponent = Root;

        MonsterSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterSkeletalMesh"));
        MonsterSkeletalMeshComponent->AttachTo(Root);

        MonsterSkeletalMeshComponent->SetSkeletalMesh(MonsterSkeletalMesh, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("monster skeletal mesh absent"));
    }
	MonsterSkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if (monsterType == DefaultMonster) {
		static ConstructorHelpers::FClassFinder<UAnimInstance> GHOUL_ANIM(TEXT("/Game/MonsterBP/GhoulAnimationBP.GhoulAnimationBP_C"));
		if (GHOUL_ANIM.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("GHOUL_ANIM Succeed"));
			MonsterSkeletalMeshComponent->SetAnimInstanceClass(GHOUL_ANIM.Class);
		}
	}
}

void AMonster::ChargePanicRoom()
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

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonster::RestoreAngry, 5.0f, false);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonster::ActiveAngry, ChargeDelay, false);
	/*TimerDel.BindUFunction(this, FName("RestoreAngry"));
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.0f, false);*/
	return;
}

void AMonster::RestoreAngry()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
	IsAngry = false;
	IsCharge = false;
	MonsterAnimInstance->SetAngry(false);
	StopCharge();
	SetActorLocation(chargeLocation);
	UE_LOG(LogTemp, Warning, TEXT("restore to %f %f"), chargeLocation.X, chargeLocation.Y);

}

void AMonster::ActiveAngry()
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	IsCharge = true;
	MonsterAnimInstance->SetAngry(false);
	MonsterAnimInstance->SetMovement(true);

}

void AMonster::EnterPanicRoom()
{
    // UE_LOG(LogTemp, Warning, TEXT("monster entered panic room!"));
	IsCharge = false;
	IsMoving = false;
    return;
}

bool AMonster::IsDoorOpen()
{
	if (!(roomControl->IsBlocked(monsterId))) {
		//UE_LOG(LogTemp, Warning, TEXT("panic room is blocked!"));
	}
	return !(roomControl->IsBlocked(monsterId));
}

void AMonster::InitMonsterActor(ARoomControl *_roomControl, int _monsterId, MonsterType _monsterType)
{
	monsterId = _monsterId;
	monsterType = _monsterType;
	roomControl = _roomControl;
	return;
}

void AMonster::MoveTo(FVector _destination)
{
	IsMoving = true;
	destination = _destination;
	chargeDirection = _destination - GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("move to %f %f"), chargeDirection.X, chargeDirection.Y);
	if (!IsAngry) {
		MonsterAnimInstance->SetMovement(true);
	}
}

void AMonster::StopCharge()
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
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	destination = GetActorLocation();
	MonsterAnimInstance = Cast<UMonsterAnimInstance>(MonsterSkeletalMeshComponent->GetAnimInstance());

	if (MonsterAnimInstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("monster is null!"));
	}

}

void AMonster::InitMonster(MonsterType t, int _monsterId)
{
    monsterId = _monsterId;

    if (t == DefaultMonster)
    {
        radioactive = false;
        emp = false;
        armorpierce = false;
        speed = 1;
    }

    prevDirection = NoDirection;
}

const int AMonster::MonsterId() const
{
    return monsterId;
}

const MonsterType AMonster::Type() const
{
    return monsterType;
}

const int AMonster::Speed() const
{
    return speed;
}

const MonsterProperty AMonster::Property() const
{
    return MakeTuple(radioactive, emp, armorpierce);
}

const Direction AMonster::PreviousDirection() const
{
    return prevDirection;
}

void AMonster::PreviousDirection(Direction d)
{
    prevDirection = d;
}
