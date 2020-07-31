// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterActor.h"
#include "RoomControl.h"

// Sets default values
AMonsterActor::AMonsterActor()
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
    // UE_LOG(LogTemp, Warning, TEXT("monster is happy!"));
    IsAngry = false;
    MonsterSkeletalMeshComponent->SetAnimInstanceClass(AnimationBPs[0]->GetAnimBlueprintGeneratedClass());
}

void AMonsterActor::EnterPanicRoom()
{
    // UE_LOG(LogTemp, Warning, TEXT("monster entered panic room!"));
    panicRoomLocation = GetActorLocation();
    return;
}

bool AMonsterActor::IsDoorOpen()
{
    if (!(roomControl->IsBlocked(id)))
    {
        // UE_LOG(LogTemp, Warning, TEXT("panic room is blocked!"));
    }
    return !(roomControl->IsBlocked(id));
}

void AMonsterActor::InitMonsterActor(ARoomControl *_roomControl, int _monsterId)
{
    id = _monsterId;
    roomControl = _roomControl;
    return;
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
    Super::BeginPlay();

    panicRoomLocation = FVector(9000.0f, 0.0f, 200.0f);
}

void AMonsterActor::InitMonster(MonsterType t, int _id)
{
    id = _id;

    if (t == DefaultMonster)
    {
        radioactive = false;
        emp = false;
        armorpierce = false;
        speed = 1;
    }

    prevDirection = NoDirection;
}

const int AMonsterActor::MonsterId() const
{
    return id;
}

const MonsterType AMonsterActor::Type() const
{
    return type;
}

const int AMonsterActor::Speed() const
{
    return speed;
}

const MonsterProperty AMonsterActor::Property() const
{
    return MakeTuple(radioactive, emp, armorpierce);
}

const Direction AMonsterActor::PreviousDirection() const
{
    return prevDirection;
}

void AMonsterActor::PreviousDirection(Direction d)
{
    prevDirection = d;
}
