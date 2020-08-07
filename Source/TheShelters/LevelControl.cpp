// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelControl.h"
#include "Monster.h"

// Sets default values
ALevelControl::ALevelControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelControl::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("1"));
}

bool ALevelControl::MyContains(int input_num)
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
        if (input_num == panicRoomId)
        {
            return true;
        }
    }
    return false;
}

bool ALevelControl::IsNextPanicRoom(int roomNumber)
{
    if (roomNumber == panicRoomId - 1 || roomNumber == panicRoomId + 1 || roomNumber == panicRoomId + 10)
    {
        UE_LOG(LogTemp, Warning, TEXT("next to room %d"), panicRoomId);
        return true;
    }
    return false;
}

void ALevelControl::InitCCTV(TArray<AActor *> _ZapPlanes, TArray<AActor *> _RoomActors)
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

Direction ALevelControl::ChooseWeightedRandomDirection(TMap<Direction, int32> weights)
{
    // Calculate cumulative sum of weights
    int32 total = 0;
    for (const TPair<Direction, int32> &it : weights)
    {
        total += it.Value;
    }

    // Get random number
    int32 r = rand() % total;

    // Randomly choose from weighted directions
    int32 csum = 0; // cumulative sum
    for (const TPair<Direction, int32> &it : weights)
    {
        csum += it.Value;
        if (csum >= r)
        {
            return it.Key;
        }
    }

    throw "Cannot move...";
    return NoDirection;
}

void ALevelControl::EndTurn()
{
    // Move monsters in map
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        TMap<Direction, int32> weights = { {Left, 6}, {Right, 6}, {Up, 6}, {Down, 6}, {NoDirection, 1} };

		/*UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT START"));
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: Left %s"), weights[Left]);
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: Right %s"), weights[Right]);
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: Up %s"), weights[Up]);
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: Down %s"), weights[Down]);

		int32 currentKey = it.Key;
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: [ID] %s"), currentKey);
		AMonster *currentMonster = monsters[currentKey];
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: [ID FROM MAP] %s"), currentMonster->MonsterId());
		Direction prevDirection = currentMonster->PreviousDirection();
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: [PREVDIRECTION] %s"), prevDirection);

		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: [BEFORE] %s"), weights[prevDirection]);
		weights[prevDirection] = 1;
		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT: [AFTER] %s"), weights[prevDirection]);

		UE_LOG(LogTemp, Warning, TEXT("@@@@@ DEBUG POINT END"));*/


        bool success;
        do
        {
            Direction moveDirection = ChooseWeightedRandomDirection(weights);
            success = MoveMonster(it.Key, moveDirection);
            if (!success)
            {
                weights[moveDirection] = 0;
            }
        } while (!success);
        
    }

    survivorStat->EndTurn();

    int mentality = survivorStat->Mental();
    if (!eventFlag["DefaultEvent"] && mentality < 100)
    {
        eventFlag["DefaultEvent"] = true;
        UE_LOG(LogTemp, Warning, TEXT("************************EVENT CALL"));
    }
}

void ALevelControl::InitGame(const unsigned int m, const unsigned int n, FString _LevelString)
{
    maxHeight = m;
    maxWidth = n;

    eventFlag.Add("DefaultEvent", false);

    this->InitRooms();
    this->InitSurvivorStat();
    this->InitMap(_LevelString);
}

void ALevelControl::InitRooms()
{
    int size = maxHeight * maxWidth;

    for (int i = 0; i < size; i++)
    {
        URoom *NewRoom = NewObject<URoom>();
        NewRoom->InitRoom(i);
        GameMap.Add(NewRoom);
    }

    // Connect all rooms, set default resosurces
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

		GameMap[i]->InitResources(None);
    }

	int maxRoomNum = maxWidth * maxHeight;
	// Set random resources
	for (int i = 0; i < maxFoodRoom; ++i) {
		int roomNum = rand() % maxRoomNum;
		while (foodRoomNum.Contains(roomNum)) {
			roomNum = rand() % maxRoomNum;
		}
		if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) + abs((int)(roomNum%maxWidth - panicRoomId % maxWidth)) >= resourceRoomDistance) {
			GameMap[roomNum]->InitResources(Food);
			foodRoomNum.Add(roomNum);
		}
	}

	for (int i = 0; i < maxWaterRoom; ++i) {
		int roomNum = rand() % maxRoomNum;
		while (foodRoomNum.Contains(roomNum) || waterRoomNum.Contains(roomNum)) {
			roomNum = rand() % maxRoomNum;
		}
		if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) + abs((int)(roomNum%maxWidth - panicRoomId % maxWidth)) >= resourceRoomDistance) {
			GameMap[roomNum]->InitResources(Water);
			waterRoomNum.Add(roomNum);
		}
	}

	for (int i = 0; i < maxElectricityRoom; ++i) {
		int roomNum = rand() % maxRoomNum;
		while (foodRoomNum.Contains(roomNum) || waterRoomNum.Contains(roomNum) || electricityRoomNum.Contains(roomNum)) {
			roomNum = rand() % maxRoomNum;
		}
		if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) + abs((int)(roomNum%maxWidth - panicRoomId % maxWidth)) >= resourceRoomDistance) {
			GameMap[roomNum]->InitResources(Electricity);
			electricityRoomNum.Add(roomNum);
		}
	}
}

void ALevelControl::InitMap(FString _LevelString)
{
    FString LevelString = FString(_LevelString);
    TArray<FString> ParsedLines;
    int row = 0;
    LevelString.ParseIntoArray(ParsedLines, LINE_TERMINATOR, true);
    UE_LOG(LogTemp, Warning, TEXT("parse complete"))
    for (const FString &it : ParsedLines)
    {
        TArray<FString> ParsedWords;
        it.ParseIntoArray(ParsedWords, TEXT(" "), true);
        // when reading row is up, down info
        if (row % 2 == 0)
        {
            for (int i = 0; i < 10; ++i)
            {

                int currentRoomId = (row / 2) * maxWidth + i;
                DoorStatus currentDoorStatus;

                // 1 means closed door
                if (ParsedWords[i].Equals("1"))
                {
                    currentDoorStatus = Close;
                }
                else
                {
                    currentDoorStatus = Open;
                }

                // close current room's UP door
                GameMap[currentRoomId]->SetDoor(Up, currentDoorStatus);
                // nullptr means door leads to map boundary, no need to change status
                if (GameMap[currentRoomId]->GetDoor(Up).connectedRoom != nullptr)
                {
                    GameMap[currentRoomId]->GetDoor(Up).connectedRoom->SetDoor(Down, currentDoorStatus);
                }
            }
        }
        // when reading row is left, right info
        else
        {
            for (int i = 0; i < 10; ++i)
            {
                // UE_LOG(LogTemp, Warning, TEXT("current pos: %s"), it[col])

                int currentRoomId = (row / 2) * maxWidth + i;
                DoorStatus currentDoorStatus;
                // 1 means closed door
                if (ParsedWords[i].Equals("1"))
                {
                    currentDoorStatus = Close;
                }
                else
                {
                    currentDoorStatus = Open;
                }

                // close current room's UP door
                GameMap[currentRoomId]->SetDoor(Left, currentDoorStatus);
                // nullptr means door leads to map boundary, no need to change status
                if (GameMap[currentRoomId]->GetDoor(Left).connectedRoom != nullptr)
                {
                    GameMap[currentRoomId]->GetDoor(Left).connectedRoom->SetDoor(Right, currentDoorStatus);
                }
            }
        }
        ++row;
    }
}

void ALevelControl::InitDoorMesh()
{
    for (int i = 0; i < 12; ++i)
    {
        SpawnDoorMesh(CCTVRoomNum[i]);
    }

    int visibleRoomSize = VisibleRoomNum.Num();
    for (int i = 0; i < visibleRoomSize; ++i)
    {
        SpawnDoorMesh(VisibleRoomNum[i]);
    }
}

void ALevelControl::InitSurvivorStat()
{
    this->survivorStat = NewObject<USurvivorStat>();
    this->survivorStat->InitSurvivorStat(100, 100, 100, 100, 100, 50, 100);
}

void ALevelControl::SpawnRoomMesh(int roomNum)
{
    int row = roomNum / maxWidth;
    int col = roomNum % maxWidth;
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;

    FRotator rotator(0.0f, 0.0f, 0.0f);

    FVector spawnLocation(startX + col * interval, startY + row * interval, startZ);
    ARoomActor *roomActor = GetWorld()->SpawnActor<ARoomActor>(RoomActor, spawnLocation, rotator, spawnParams);
    roomActor->RoomMeshRandomize();
}

void ALevelControl::SpawnDoorMesh(int roomNum)
{
    UWorld *world = GetWorld();
    int row = roomNum / maxWidth;
    int col = roomNum % maxWidth;
    UE_LOG(LogTemp, Warning, TEXT("roomnum %d, coord [%d : %d]"), roomNum, row, col);
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;

    FRotator rotatorLeft(0.0f, 0.0f, 0.0f);
    FRotator rotatorUp(0.0f, 90.0f, 0.0f);

    Door currentDoor;
    FVector spawnLocationUp(startX + col * interval, startY + row * interval - 305.0f, startZ);
    currentDoor = GameMap[roomNum]->GetDoor(Up);
    if (currentDoor.connectedRoom != nullptr && currentDoor.status == Open)
    {

        world->SpawnActor<ADoorActor>(DoorActor[0], spawnLocationUp, rotatorUp, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoorActor>(DoorActor[1], spawnLocationUp, rotatorUp, spawnParams);
    }

    FVector spawnLocationLeft(startX + col * interval - 305.0f, startY + row * interval, startZ);
    currentDoor = GameMap[roomNum]->GetDoor(Left);
    if (currentDoor.connectedRoom != nullptr && currentDoor.status == Open)
    {

        world->SpawnActor<ADoorActor>(DoorActor[0], spawnLocationLeft, rotatorLeft, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoorActor>(DoorActor[1], spawnLocationLeft, rotatorLeft, spawnParams);
    }

    FVector spawnLocationRight(startX + col * interval + 305.0f, startY + row * interval, startZ);
    currentDoor = GameMap[roomNum]->GetDoor(Right);
    if (currentDoor.connectedRoom != nullptr && currentDoor.status == Open)
    {

        world->SpawnActor<ADoorActor>(DoorActor[0], spawnLocationRight, rotatorLeft, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoorActor>(DoorActor[1], spawnLocationRight, rotatorLeft, spawnParams);
    }

    FVector spawnLocationDown(startX + col * interval, startY + row * interval + 305.0f, startZ);
    currentDoor = GameMap[roomNum]->GetDoor(Down);
    if (currentDoor.connectedRoom != nullptr && currentDoor.status == Open)
    {

        world->SpawnActor<ADoorActor>(DoorActor[0], spawnLocationDown, rotatorUp, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoorActor>(DoorActor[1], spawnLocationDown, rotatorUp, spawnParams);
    }
}

// calculate and add visible room numbers
void ALevelControl::InitVisibleRoom()
{

    // Left side visible rooms
    URoom *currentRoom = GameMap[panicRoomId - 1];
    Door currentDoor = currentRoom->GetDoor(Left);
    int currentRoomNum = panicRoomId - 1;

    UE_LOG(LogTemp, Warning, TEXT("visible rooms"));

    while (1)
    {

        // spawn room if cctv has not spawned room for current room
        if (!CCTVRoomNum.Contains(currentRoomNum))
        {
            SpawnRoomMesh(currentRoomNum);
            VisibleRoomNum.Add(currentRoomNum);
        }
        if (currentRoom == nullptr || currentDoor.connectedRoom == nullptr || currentDoor.status == Close)
        {
            break;
        }

        currentRoom = currentDoor.connectedRoom;
        currentDoor = currentRoom->GetDoor(Left);

        currentRoomNum--;
    }

    currentRoom = GameMap[panicRoomId + 10];
    currentDoor = currentRoom->GetDoor(Down);
    currentRoomNum = panicRoomId + 10;

    while (1)
    {

        // spawn room if cctv has not spawned room for current room
        if (!CCTVRoomNum.Contains(currentRoomNum))
        {
            SpawnRoomMesh(currentRoomNum);
            VisibleRoomNum.Add(currentRoomNum);
        }
        if (currentRoom == nullptr || currentDoor.connectedRoom == nullptr || currentDoor.status == Close)
        {
            break;
        }

        currentRoom = currentDoor.connectedRoom;
        currentDoor = currentRoom->GetDoor(Down);

        currentRoomNum += 10;
    }

    currentRoom = GameMap[panicRoomId + 1];
    currentDoor = currentRoom->GetDoor(Right);
    currentRoomNum = panicRoomId + 1;

    while (1)
    {

        // spawn room if cctv has not spawned room for current room
        if (!CCTVRoomNum.Contains(currentRoomNum))
        {
            UE_LOG(LogTemp, Warning, TEXT("visible room Right %d"), currentRoomNum);
            SpawnRoomMesh(currentRoomNum);
            VisibleRoomNum.Add(currentRoomNum);
        }
        if (currentRoom == nullptr || currentDoor.connectedRoom == nullptr || currentDoor.status == Close)
        {
            break;
        }

        currentRoom = currentDoor.connectedRoom;
        currentDoor = currentRoom->GetDoor(Right);

        currentRoomNum++;
    }
}
bool ALevelControl::CheckPanicRoom(int _monsterId)
{
	for (const TPair<int32, int32> &it : monsterLocations)
	{
		if (it.Key == _monsterId)
		{
			if (IsNextPanicRoom(it.Value))
			{
				UE_LOG(LogTemp, Warning, TEXT("next to room %d"), panicRoomId);
				monsterActors[_monsterId - 1]->MoveTo(FVector(startX + interval * (panicRoomId % maxWidth), startY + interval * (panicRoomId / maxWidth), startZ));
				monsterActors[_monsterId - 1]->ChargePanicRoom();
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

// Returns resource type, resource size. resource type = 0 for not discovered or not known
TArray<FResourceUI> ALevelControl::GetRoomResourceUI()
{
	TArray<FResourceUI> ResourceArray;
	URoom* currentRoom;
	
	int maxRoomNum = maxWidth * maxHeight;
	for (int i = 0; i < maxRoomNum; ++i) {
		currentRoom = GameMap[i];
		FResourceUI currentResourceUI;

		if (currentRoom->isDiscovered()) {

			// find maximum resource type and its size
			Resource currentResource = currentRoom->GetResources();
			if (currentResource.food >= currentResource.water) {
				if (currentResource.food >= currentResource.electricity) {
					currentResourceUI.resourceType = 1;
					currentResourceUI.resourceSize = currentResource.food;
				}
				else {
					currentResourceUI.resourceType = 3;
					currentResourceUI.resourceSize = currentResource.electricity;
				}
			}
			else if (currentResource.water >= currentResource.electricity) {
				currentResourceUI.resourceType = 2;
				currentResourceUI.resourceSize = currentResource.water;
			}
			else {
				currentResourceUI.resourceType = 3;
				currentResourceUI.resourceSize = currentResource.electricity;
			}
		}
		else {
			// undiscovered room
			currentResourceUI.resourceType = 0;
			currentResourceUI.resourceSize = 0;
		}
		ResourceArray.Add(currentResourceUI);
	}
	
	return ResourceArray;
}

TArray<int> ALevelControl::GetDoorUI()
{
	TArray<int> DoorArray;
	int currentRoomNum;
	for (unsigned int i = 0; i < maxHeight; ++i) {
		for (unsigned int j = 0; j < maxWidth; ++j) {
			currentRoomNum = i * maxWidth + j;
			if (j == 0) {
				DoorArray.Add(1);
			}
			else {
				DoorArray.Add(GameMap[currentRoomNum]->GetDoor(Left).status);
			}

			if (i == 0) {
				DoorArray.Add(1);
			}
			else {
				DoorArray.Add(GameMap[currentRoomNum]->GetDoor(Up).status);
			}
		}
	}
	return DoorArray;
}

bool ALevelControl::IsBlocked(int _monsterId)
{
    Door door;
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (it.Key == _monsterId)
        {
            int roomNumber = it.Value;

            if (roomNumber == panicRoomId - 1)
            {
                door = GameMap[panicRoomId]->GetDoor(Left);
            }
            else if (roomNumber == panicRoomId + 1)
            {
                door = GameMap[panicRoomId]->GetDoor(Right);
            }
            else if (roomNumber == panicRoomId + 10)
            {
                door = GameMap[panicRoomId]->GetDoor(Down);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("monster %d roomNumber error!"), _monsterId);
                return false;
            }
            break;
        }
    }
    if (door.status == Close)
    {
        return true;
    }
    else
    {
        return false;
    }
}

URoom *ALevelControl::FindRoomByLocation(const unsigned int x, const unsigned int y)
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

URoom *ALevelControl::FindRoomById(const int roomId)
{
    return GameMap[roomId];
}

void ALevelControl::InsertMonster(MonsterType monsterType, int x, int y)
{
    int roomId = x * maxWidth + y;
    InsertMonster(monsterType, roomId);
}

void ALevelControl::InsertMonster(MonsterType monsterType, int roomId)
{
    int row = roomId / maxWidth;
    int col = roomId % maxWidth;

    GameMap[roomId]->InsertMonster(nextMonsterId);

    AMonster *newMonster = NewObject<AMonster>();
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
            FVector spawnLocation(startX + col * interval, startY + row * interval, startZ);

            monsterActors.Add(world->SpawnActor<AMonster>(MonsterSpawn[monsterType], spawnLocation, rotator, spawnParams));
            monsterActors[nextMonsterId - 1]->InitMonsterActor(this, nextMonsterId, monsterType);
        }
    }
    nextMonsterId++;
}

void ALevelControl::DeleteMonster(const unsigned int x, const unsigned int y)
{
    URoom *room = FindRoomByLocation(x, y);
    room->DeleteMonster();
}

void ALevelControl::DeleteMonster(int roomId)
{
    URoom *room = FindRoomById(roomId);
    room->DeleteMonster();
}

bool ALevelControl::MoveMonster(int monsterId, Direction d)
{
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (it.Key == monsterId)
        {
            if (d == NoDirection)
            {
                monsters[it.Key]->PreviousDirection(NoDirection);
                return true;
            }
            if (monsterActors[monsterId - 1]->IsAngry)
            {
                continue;
            }

            Door door = GameMap[it.Value]->GetDoor(d);

            if (door.connectedRoom == nullptr || door.status == Close ||
                GameMap[door.connectedRoom->RoomId()]->MonsterId() != 0 || door.connectedRoom->RoomId() == panicRoomId)
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
				//monsterActors[monsterId - 1]->SetActorLocation(FVector(startX + interval * (it.Value % maxWidth), startY + interval * (it.Value / maxWidth), startZ));
				monsterActors[monsterId - 1]->MoveTo(FVector(startX + interval * (it.Value % maxWidth), startY + interval * (it.Value / maxWidth), startZ));
			}

            Direction prev = static_cast<Direction>((d + 2) % 4);
            monsters[it.Key]->PreviousDirection(prev);
            break;
        }
    }
    return true;
}

AMonster *ALevelControl::FindMonsterById(const unsigned int id)
{

    for (const TPair<int32, AMonster *> &it : monsters)
    {
        if (it.Key == id)
        {
            UE_LOG(LogTemp, Warning, TEXT("Hello pretty monster"));
            AMonster *m = it.Value;

            return m;
        }
    }
    return NULL;
}

void ALevelControl::ZapCCTV(AActor *_CurrentZapPlane)
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

void ALevelControl::RestoreZap(AActor *CCTV)
{
    CCTV->SetActorHiddenInGame(true);
}

void ALevelControl::SelectCCTV()
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


int ALevelControl::ResourceCheckByRobot(int RoomId, int Type)
{
    if (Type == 1)
    {
        return GameMap[RoomId]->GetResources().food;
    }
    else if (Type == 2)
    {
        return GameMap[RoomId]->GetResources().water;
    }
    else if (Type == 3)
    {
        return GameMap[RoomId]->GetResources().electricity;
    }
    return 0;
}

void ALevelControl::SetRoomResources(int RoomId, int food, int water, int electricity)
{
    GameMap[RoomId]->SetResources(food, water, electricity);
}

void ALevelControl::RobotCheck(int RoomId)
{
    GameMap[RoomId]->SetisKnown(true);
}


bool ALevelControl::IsRoomClosed(int roomNum, int direction) //For RobotControl Usage. 1 = up, 2 = right, 3 = down, 4 = left
{
    
    Door door;
    if (direction == 1)
    {
        door = GameMap[roomNum]->GetDoor(Up);
    }
    else if (direction == 2)
    {
        door = GameMap[roomNum]->GetDoor(Right);
    }
    else if (direction == 3)
    {
        door = GameMap[roomNum]->GetDoor(Down);
    }
    else if (direction == 4)
    {
        door = GameMap[roomNum]->GetDoor(Left);
    }
    
    if (door.status == Close)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Get functions

TArray<int32> ALevelControl::GetCCTVRoomNum()
{
	return CCTVRoomNum;
}

int ALevelControl::GetMaxWidth()
{
	return maxWidth;
}

int ALevelControl::GetMaxHeight()
{
	return maxHeight;
}