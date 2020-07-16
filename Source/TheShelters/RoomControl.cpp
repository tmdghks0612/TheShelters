// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomControl.h"

// Sets default values
ARoomControl::ARoomControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoomControl::BeginPlay()
{
    Super::BeginPlay();
}

void ARoomControl::PrintMap()
{
    for (unsigned int x = 0; x < maxHeight; x++)
    {
        FString line = FString();
        for (unsigned int y = 0; y < maxWidth; y++)
        {
            int idx = x * maxWidth + y;
            int32 monsterId = GameMap[idx]->MonsterId();
            TArray<FStringFormatArg> args;
            args.Add(FStringFormatArg(monsterId));
            line += FString::Format(TEXT("{0} "), args);
        }
        UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
    }
}

void ARoomControl::PrintTestMessage(const TCHAR *testName, const int num, const bool success)
{
    FString line = FString();
    TArray<FStringFormatArg> args;
    args.Add(FStringFormatArg(testName));
    args.Add(FStringFormatArg(num));
    if (success)
    {
        line += FString::Format(TEXT("[{0}] TEST {1}: SUCCESS"), args);
    }
    else
    {
        line += FString::Format(TEXT("[{0}] TEST {1}: FAIL"), args);
    }

    UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
}

void ARoomControl::TestScenario()
{
    InitGame(10, 10);
    InsertMonster(DefaultMonster, 0, 4); // Monster 1: 0, 4
    InsertMonster(DefaultMonster, 1, 3); // Monster 2: 1, 3

    MoveMonster(1, Left);  // Monster 1: 0, 3
    MoveMonster(2, Right); // Monster 2: 1, 4

    // EXPECT
    bool result;
    result = GameMap[4]->MonsterId() == 0 && GameMap[3]->MonsterId() == 1;
    PrintTestMessage(TEXT("MonsterMovement"), 1, result);

    result = GameMap[13]->MonsterId() == 0 && GameMap[14]->MonsterId() == 2;
    PrintTestMessage(TEXT("MonsterMovement"), 2, result);

    URoom *room = FindRoomByLocation(3, 3);
    room->CloseDoor(Left);
    result = GameMap[33]->GetDoor(Left).status == Close;
    PrintTestMessage(TEXT("CloseDoor"), 1, result);

    result = GameMap[32]->GetDoor(Right).status == Close;
    PrintTestMessage(TEXT("CloseDoor"), 2, result);
}

void ARoomControl::InitCCTV(TArray<AActor*> _ZapPlanes)
{
    for (int i = 0; i< 9; ++i) {
        ZapPlanes.Add(_ZapPlanes[i]);

        while (CCTVRoomNum.AddUnique(rand() % (maxWidth * maxHeight)) == -1);
    }
    for (int i = 0; i < 9; ++i) {
        ZapPlanes[i]->SetActorHiddenInGame(true);
        UE_LOG(LogTemp, Warning, TEXT("loop: %d"), CCTVRoomNum[i]);

void ARoomControl::EndTurn()
{
}

void ARoomControl::InitGame(const unsigned int m, const unsigned int n)
{
    maxHeight = m;
    maxWidth = n;

    int size = maxHeight * maxWidth;

    for (int i = 0; i < size; i++)
    {
        URoom *NewRoom = NewObject<URoom>();
        NewRoom->InitRoom(i);
        GameMap.Add(NewRoom);
    }

    // Connect all rooms
    for (int i = 0; i < size; i++)
    {
        // A idx is MaxWidth * x + y
        int x = i / maxWidth;
        int y = i % maxWidth;

        URoom *room;
        if (x != 0)
        {
            room = FindRoomByLocation(x - 1, y);
            GameMap[i]->InitDoor(Up, room, Open);
        }
        if (x != (maxHeight - 1))
        {
            room = FindRoomByLocation(x + 1, y);
            GameMap[i]->InitDoor(Down, room, Open);
        }
        if (y != 0)
        {
            room = FindRoomByLocation(x, y - 1);
            GameMap[i]->InitDoor(Left, room, Open);
        }
        if (y != (maxWidth - 1))
        {
            room = FindRoomByLocation(x, y + 1);
            GameMap[i]->InitDoor(Right, room, Open);
        }
    }
}

URoom *ARoomControl::FindRoomByLocation(const unsigned int x, const unsigned int y)
{
    if (x < 0 || x >= maxHeight)
    {
        throw "You've exceeded map height";
    }
    if (y < 0 || y >= maxWidth)
    {
        throw "You've exceeded map width";
    }

    int idx = maxWidth * x + y;

    return GameMap[idx];
}

URoom *ARoomControl::FindRoomById(const int roomId)
{
    return GameMap[roomId];
}

void ARoomControl::InsertMonster(MonsterType monsterType, int x, int y)
{
    int roomNum = x * maxWidth + y;
    InsertMonster(monsterType, roomNum);
}

void ARoomControl::InsertMonster(MonsterType monsterType, int roomId)
{
    GameMap[roomId]->InsertMonster(nextMonsterId);

    UMonster *newMonster = NewObject<UMonster>();
    newMonster->InitMonster(monsterType, nextMonsterId);
    monsters.Add(nextMonsterId, newMonster);
    monsterLocations.Add(nextMonsterId, roomId);
    nextMonsterId++;
}

void ARoomControl::DeleteMonster(const unsigned int x, const unsigned int y)
{
    URoom *room = FindRoomByLocation(x, y);
    room->DeleteMonster();
}

void ARoomControl::DeleteMonster(int roomId)
{
    URoom *room = FindRoomById(roomId);
    room->DeleteMonster();
}

bool ARoomControl::MoveMonster(int monsterId, Direction d)
{
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (it.Key == monsterId)
        {
            Door door = GameMap[it.Value]->GetDoor(d);

            if (door.connectedRoom == nullptr || door.status == Close ||
                GameMap[door.connectedRoom->RoomId()]->MonsterId() != 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
                return false;
            }
            UE_LOG(LogTemp, Warning, TEXT("monster %d moving from room %d..."), it.Key, it.Value);
            GameMap[it.Value]->DeleteMonster();
            monsterLocations[it.Key] = door.connectedRoom->RoomId();
            GameMap[it.Value]->InsertMonster(monsterId);
            break;
        }
    }
    return true;
}

UMonster *ARoomControl::FindMonsterById(const unsigned int id)
{

    for (const TPair<int32, UMonster *> &it : monsters)
    {
        if (it.Key == id)
        {
            UE_LOG(LogTemp, Warning, TEXT("Hello pretty monster"));
            UMonster *m = it.Value;

            return m;
        }
    }
    return NULL;
}

void ARoomControl::ZapCCTV()
{
    FTimerDelegate TimerDel;
    FTimerHandle TimerHandle;

    int zapped = rand() % 9;
    UE_LOG(LogTemp, Warning, TEXT("CCTV set visibility %d"), zapped);

    ZapPlanes[zapped]->SetActorHiddenInGame(false);

    TimerDel.BindUFunction(this, FName("RestoreZap"), ZapPlanes[zapped]);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.2f, false);
}

void AShelterControl::RestoreZap(AActor* CCTV)
{
    CCTV->SetActorHiddenInGame(true);
    UE_LOG(LogTemp, Warning, TEXT("CCTV enabled"));
}
