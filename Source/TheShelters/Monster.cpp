// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "LevelControl.h"

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
        MonsterSkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);

        MonsterSkeletalMeshComponent->SetSkeletalMesh(MonsterSkeletalMesh, true);

		MonsterAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MonsterAudio"));
		MonsterAudioComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
		// I want the sound to come from slighty in front of the pawn.
		MonsterAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("monster skeletal mesh absent"));
    }
	MonsterSkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if (monsterType == MonsterType::Ghoul) {
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
	FTimerHandle TimerHandleRestore;
	UE_LOG(LogTemp, Warning, TEXT("monster is angry!"));
	chargeLocation = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("saving on %f %f"), chargeLocation.X, chargeLocation.Y);
	IsAngry = true;
	MonsterAnimInstance->SetAngry(true);
	IsMoving = false;
	MonsterAnimInstance->SetMovement(false);

	GetWorldTimerManager().SetTimer(TimerHandleRestore, this, &AMonster::RestoreAngry, 10.0f, false);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonster::ActiveAngry, ChargeDelay, false);

	MonsterAudioComponent->SetSound(MonsterAngrySound);
	MonsterAudioComponent->Play();
	return;
}

void AMonster::RestoreAngry()
{
	//UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
	IsNear = false;
	IsAngry = false;
	IsCharge = false;
	MonsterAnimInstance->SetAngry(false);
	
	IsMoving = false;
	MonsterAnimInstance->SetMovement(false);

	waitTime = actionFrequency;
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

bool AMonster::EnterPanicRoom()
{
    UE_LOG(LogTemp, Warning, TEXT("monster entered panic room! You DIED!"));
	IsCharge = false;
	IsMoving = false;

	GetWorldTimerManager().ClearAllTimersForObject(this);

	MonsterAnimInstance->SetAngry(true);
	MonsterAnimInstance->SetMovement(false);

	if (LevelControl->IsGameOver) {
		return false;
	}
	LevelControl->GameOver();
    return true;
}

bool AMonster::IsDoorOpen()
{
	if (!(LevelControl->IsBlocked(monsterId))) {
		//UE_LOG(LogTemp, Warning, TEXT("panic room is blocked!"));
	}
	return !(LevelControl->IsBlocked(monsterId));
}

bool AMonster::IsReadyToMove()
{
	waitTime++;
	if (waitTime > actionFrequency) {
		waitTime = 0;
		return true;
	}
	else {
		return false;
	}
}

void AMonster::InitMonsterActor(ALevelControl *_LevelControl, int _monsterId, MonsterType _monsterType)
{
	monsterId = _monsterId;
	monsterType = _monsterType;
	LevelControl = _LevelControl;
	return;
}

void AMonster::MoveTo(FVector _destination)
{
	IsNear = false;
	IsMoving = true;
	destination = _destination;
	chargeDirection = _destination - GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("move to %f %f"), chargeDirection.X, chargeDirection.Y);
	if (!IsAngry) {
		MonsterAnimInstance->SetMovement(true);
	}
	MonsterAudioComponent->SetSound(MonsterMovementSound);
	MonsterAudioComponent->Play();
}

void AMonster::StopCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("stopped!!"));
	IsMoving = false;
	IsCharge = false;
	MonsterAnimInstance->SetMovement(false);

	if (LevelControl->CheckPanicRoom(monsterId)) {
		IsNear = true;
	}
}

void AMonster::Wait()
{
	IsMoving = false;
	IsCharge = false;
	MonsterAnimInstance->SetMovement(false);
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

    if (t == MonsterType::Ghoul)
    {
        radioactive = false;
        emp = false;
        armorpierce = false;
        speed = 1;
    }

    prevDirection = Direction::None;
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
