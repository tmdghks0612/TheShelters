// Fill out your copyright notice in the Description page of Project Settings.


#include "ShelterControl.h"

// Sets default values
AShelterControl::AShelterControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShelterControl::BeginPlay()
{
    Super::BeginPlay();
}

void AShelterControl::PrintMap()
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

void AShelterControl::PrintTestMessage(const TCHAR* testName, const int num, const bool success)
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

void AShelterControl::TestScenario()
{
	InitGame(10, 10);
	InsertMonster(DefaultMonster, 0, 4);  // Monster 1: 0, 4
	InsertMonster(DefaultMonster, 1, 3);  // Monster 2: 1, 3

    MoveMonster(1, Left);                 // Monster 1: 0, 3
    MoveMonster(2, Right);                // Monster 2: 1, 4

    // EXPECT
    bool result;
    result = GameMap[4]->MonsterId() == 0 && GameMap[3]->MonsterId() == 1;
    PrintTestMessage(TEXT("MonsterMovement"), 1, result);

    result = GameMap[8]->MonsterId() == 0 && GameMap[9]->MonsterId() == 2;
    PrintTestMessage(TEXT("MonsterMovement"), 2, result);
}

void AShelterControl::InitCCTV(TArray<AActor*> _ZapPlanes)
{
	for (int i = 0; i < 9; ++i) {
		ZapPlanes.Add(_ZapPlanes[i]);

		while (CCTVRoomNum.AddUnique(rand() % (maxWidth * maxHeight)) == -1);
	}
	for (int i = 0; i < 9; ++i) {
		ZapPlanes[i]->SetActorHiddenInGame(true);
		UE_LOG(LogTemp, Warning, TEXT("loop : %d"), CCTVRoomNum[i]);
	}
}

void AShelterControl::EndTurn()
{
}

void AShelterControl::InitGame(const unsigned int m, const unsigned int n) {
    maxHeight = m;
    maxWidth = n;

    int size = maxHeight * maxWidth;
    
    for (int i = 0; i < size; i++) {
        UShelter *NewRoom = NewObject<UShelter>();
        NewRoom->InitShelter(i);
        GameMap.Add(NewRoom);
    }

    // Connect all rooms
    for (int i = 0; i < size; i++) {
        // A idx is MaxWidth * x + y
        int x = i / maxWidth;
        int y = i % maxWidth;

        if (x != 0) {
            GameMap[i]->InitDoor(Up, maxWidth * (x - 1) + y, Open);
        }
        if (x != (maxHeight - 1)) {
            GameMap[i]->InitDoor(Down, maxWidth * (x + 1) + y, Open);
        }
        if (y != 0) {
            GameMap[i]->InitDoor(Left, maxWidth * x + (y - 1), Open);
        }
        if (y != (maxWidth - 1)) {
            GameMap[i]->InitDoor(Right, maxWidth * x + (y + 1), Open);
        }
    }
}


UShelter* AShelterControl::FindShelterByLocation(const unsigned int x, const unsigned int y) {
    if (x < 0 || x >= maxHeight) {
        throw "You've exceeded map height";
    }
    if (y < 0 || y >= maxWidth) {
        throw "You've exceeded map width";
    }

    int idx = maxWidth * x + y;

    return GameMap[idx];
}

UShelter* AShelterControl::FindShelterById(const int shelterId)
{
    return GameMap[shelterId];
}

void AShelterControl::InsertMonster(MonsterType monsterType, int x, int y)
{
    int shelterNum = x * maxWidth + y;
    InsertMonster(monsterType, shelterNum);
}

void AShelterControl::InsertMonster(MonsterType monsterType, int shelterId)
{
    GameMap[shelterId]->InsertMonster(nextMonsterId);
    
    UMonster* newMonster = NewObject<UMonster>();
    newMonster->InitMonster(monsterType, nextMonsterId);
    monsters.Add(nextMonsterId, newMonster);
    monsterLocations.Add(nextMonsterId, shelterId);
    nextMonsterId++;
}

void AShelterControl::DeleteMonster(const unsigned int x, const unsigned int y)
{
    UShelter* room = FindShelterByLocation(x, y);
    room->DeleteMonster();
}

void AShelterControl::DeleteMonster(int shelterId)
{
    UShelter* room = FindShelterById(shelterId);
    room->DeleteMonster();
}

bool AShelterControl::MoveMonster(int monsterId, Direction d) {
    for (const TPair<int32, int32>& it : monsterLocations) {
        if (it.Key == monsterId) {
            Door door = GameMap[it.Value]->GetDoor(d);

            if (door.connectedShelter == 0 || door.status == Close || GameMap[door.connectedShelter]->MonsterId() != 0) {
                UE_LOG(LogTemp, Warning, TEXT("Cannot move!"));
                return false;
            }
            UE_LOG(LogTemp, Warning, TEXT("monster %d moving from room %d..."),it.Key, it.Value);
            GameMap[it.Value]->DeleteMonster();
            monsterLocations[it.Key] = door.connectedShelter;
            GameMap[it.Value]->InsertMonster(monsterId);
            break;
        }
    }
    return true;
}

UMonster* AShelterControl::FindMonsterById(const unsigned int id) {

    for (const TPair<int32, UMonster*>& it : monsters) {
        if (it.Key == id) {
            UE_LOG(LogTemp, Warning, TEXT("Hello pretty monster"));
            UMonster* m = it.Value;

            return m;
        }
    }
    return NULL;
}

void AShelterControl::ZapCCTV()
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	int zapped = rand()%9;
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

