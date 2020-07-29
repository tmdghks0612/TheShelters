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
    UE_LOG(LogTemp, Warning, TEXT("1"));
}

bool ARoomControl::MyContains(int input_num)
{
    int row_count = 0;
    int col_count = 0;
    for (int j = 0; j < CCTVRoomNum.Num(); ++j)
    {
        if (CCTVRoomNum[j] == input_num)
        {
            return true;
        }
        if ((CCTVRoomNum[j] / maxWidth) == (input_num / maxWidth))
        {
            if (row_count > 0)
            {
                return true;
            }
            row_count++;
        }
        if ((CCTVRoomNum[j] % maxWidth) == (input_num % maxWidth))
        {
            if (col_count > 0)
            {
                return true;
            }
            col_count++;
        }
		if (input_num == panicRoomId) {
			return true;
		}
    }
    return false;
}


bool ARoomControl::IsNextPanicRoom(int roomNumber)
{
	if (roomNumber == panicRoomId-1 || roomNumber == panicRoomId+1 || roomNumber == panicRoomId+10) {
		UE_LOG(LogTemp, Warning, TEXT("next to room %d"), panicRoomId);
		return true;
	}
	return false;
}

void ARoomControl::InitCCTV(TArray<AActor *> _ZapPlanes, TArray<AActor *> _RoomActors)
{
    CCTVRoomNum.Empty();
    for (int i = 0; i < 12; ++i)
    {
        ZapPlanes.Add(_ZapPlanes[i]);
        int input_num = rand() % (maxWidth * maxHeight);

        while (MyContains(input_num))
        {
            input_num = rand() % (maxWidth * maxHeight);
        }
        CCTVRoomNum.Add(input_num);
    }
    for (int i = 0; i < 12; ++i)
    {
        ZapPlanes[i]->SetActorHiddenInGame(true);
        FRotator rotator(0.0f, 0.0f, 0.0f);
        int row = CCTVRoomNum[i] / maxWidth;
        int col = CCTVRoomNum[i] % maxWidth;
        FVector spawnLocation(startX + col * interval, startY + row * interval, startZ);

        _RoomActors[i]->SetActorLocation(spawnLocation);
    }
}

void ARoomControl::EndTurn()
{
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (!MoveMonster(it.Key, Direction(rand() % 4)))
        {
            MoveMonster(it.Key, Direction(rand() % 4));
        }
    }

    this->playerStat->EndTurn();
}

void ARoomControl::InitGame(const unsigned int m, const unsigned int n)
{
    maxHeight = m;
    maxWidth = n;

    this->InitRooms();
    this->InitPanicRoom();
    this->InitPlayerStat();
}

void ARoomControl::InitRooms()
{
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

void ARoomControl::InitPanicRoom()
{
    // Get Panic Room Id

    UE_LOG(LogTemp, Warning, TEXT("panic room id: %d"), this->panicRoomId);
    UPanicRoom *panicRoom = NewObject<UPanicRoom>();
    panicRoom->InitRoom(panicRoomId);
    GameMap[panicRoomId] = panicRoom;

    int x = panicRoomId / maxWidth;
    int y = panicRoomId % maxHeight;
    URoom *room;

    if (x != 0)
    {
        room = FindRoomByLocation(x - 1, y);
        GameMap[panicRoomId]->InitDoor(Up, room, Open);
        GameMap[(x - 1) * maxWidth + y]->InitDoor(Down, panicRoom, Open);
    }
    if (x != (maxHeight - 1))
    {
        room = FindRoomByLocation(x + 1, y);
        GameMap[panicRoomId]->InitDoor(Down, room, Open);
        GameMap[(x + 1) * maxWidth + y]->InitDoor(Up, panicRoom, Open);
    }
    if (y != 0)
    {
        room = FindRoomByLocation(x, y - 1);
        GameMap[panicRoomId]->InitDoor(Left, room, Open);
        GameMap[x * maxWidth + (y - 1)]->InitDoor(Right, panicRoom, Open);
    }
    if (y != (maxWidth - 1))
    {
        room = FindRoomByLocation(x, y + 1);
        GameMap[panicRoomId]->InitDoor(Right, room, Open);
        GameMap[x * maxWidth + (y + 1)]->InitDoor(Left, panicRoom, Open);
    }

    panicRoom->InitPanicRoom(Close, Open, Close, Open, panicRoomId);
}


void ARoomControl::InitPlayerStat()
{
    this->playerStat = NewObject<UPlayerStat>();
    this->playerStat->InitPlayerStat(100, 100, 50, 0, 100);
}
bool ARoomControl::IsBlocked(int _monsterId)
{
	Door door;
	for (const TPair<int32, int32> &it : monsterLocations)
	{
		if (it.Key == _monsterId)
		{
			int roomNumber = it.Value;
			
			if (roomNumber == panicRoomId - 1) {
				door = GameMap[panicRoomId]->GetDoor(Left);
			}
			else if (roomNumber == panicRoomId + 1) {
				door = GameMap[panicRoomId]->GetDoor(Right);
			}
			else if (roomNumber == panicRoomId+10){
				door = GameMap[panicRoomId]->GetDoor(Down);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("monster %d roomNumber error!"), _monsterId);
				return false;
			}
			break;
		}
	}
	if (door.status == Close) {
		return true;
	}
	else {
		return false;
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
    int roomId = x * maxWidth + y;
    InsertMonster(monsterType, roomId);
}

void ARoomControl::InsertMonster(MonsterType monsterType, int roomId)
{
    int x = roomId / maxWidth;
    int y = roomId % maxWidth;

    GameMap[roomId]->InsertMonster(nextMonsterId);

    UMonster *newMonster = NewObject<UMonster>();
    newMonster->InitMonster(monsterType, nextMonsterId);
    monsters.Add(nextMonsterId, newMonster);
    monsterLocations.Add(nextMonsterId, roomId);

    if (MonsterSpawn[monsterType])
    {
        UWorld *world = GetWorld();
        if (world)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = this;

            FRotator rotator(0.0f, 0.0f, 0.0f);
            FVector spawnLocation(startX + x * interval, startY + y * interval, startZ);

            monsterActors.Add(world->SpawnActor<AMonsterActor>(MonsterSpawn[monsterType], spawnLocation, rotator, spawnParams));
			monsterActors[nextMonsterId-1]->InitMonsterActor(this, nextMonsterId);
        }
    }

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
			if (monsterActors[monsterId - 1]->IsAngry) {
				continue;
			}

            Door door = GameMap[it.Value]->GetDoor(d);

            if (door.connectedRoom == nullptr || door.status == Close ||
                GameMap[door.connectedRoom->RoomId()]->MonsterId() != 0||
				door.connectedRoom->RoomId() == panicRoomId)
            {
                return false;
            }

			// when monster successfully moving to next room
            GameMap[it.Value]->DeleteMonster();
            monsterLocations[it.Key] = door.connectedRoom->RoomId();
            GameMap[it.Value]->InsertMonster(monsterId);

			// move matching monster to next room
            if (monsterActors[monsterId - 1])
            {
                monsterActors[monsterId - 1]->SetActorLocation(FVector(
                startX + interval * (it.Value % maxWidth), startY + interval * (it.Value / maxWidth), startZ));
            }

			// if monster is next to panic room, charges to player
			if (IsNextPanicRoom(it.Value)) {
				UE_LOG(LogTemp, Warning, TEXT("next to room %d"), panicRoomId);
				monsterActors[monsterId - 1]->ChargePanicRoom();
			}
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

void ARoomControl::ZapCCTV(AActor *_CurrentZapPlane)
{
    FTimerDelegate TimerDel;
    FTimerHandle TimerHandle;

    /*int zapped = rand() % 9;
    UE_LOG(LogTemp, Warning, TEXT("CCTV set visibility %d"), zapped);

    ZapPlanes[zapped]->SetActorHiddenInGame(false);

    TimerDel.BindUFunction(this, FName("RestoreZap"), ZapPlanes[zapped]);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.2f, false);
	*/
    _CurrentZapPlane->SetActorHiddenInGame(false);
    TimerDel.BindUFunction(this, FName("RestoreZap"), _CurrentZapPlane);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.2f, false);
}

void ARoomControl::RestoreZap(AActor *CCTV)
{
    CCTV->SetActorHiddenInGame(true);
}

void ARoomControl::SelectCCTV()
{
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        for (int i = 0; i < 12; ++i)
        {
            if (CCTVRoomNum[i] == it.Value)
            {
                ZapCCTV(ZapPlanes[i]);
            }
        }
    }
}
