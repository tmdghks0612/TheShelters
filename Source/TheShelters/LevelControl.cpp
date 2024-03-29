// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelControl.h"
#include "Monster.h"

// Sets default values
ALevelControl::ALevelControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	IsGameOver = false;
	UIShowFlag = false;
}

// Called when the game starts or when spawned
void ALevelControl::BeginPlay()
{
    Super::BeginPlay();
    GameControl = Cast<UGameControl>(GetGameInstance());
}


void ALevelControl::SaveStatus()
{
    if (!(GameControl->CheckLoaded()))
    {
        if (UShelterGameSave* LoadedGame = Cast<UShelterGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("SAVE"), 0)))
        {
            for (int i = 0; i < 100; i++)
            {
                GameMap[i]->SetFood(LoadedGame->GetFood(i));
                GameMap[i]->SetWater(LoadedGame->GetWater(i));
                GameMap[i]->SetElectricity(LoadedGame->GetElect(i));

                if (LoadedGame->GetLeft(i) == true)
                {
                    GameMap[i]->OpenDoor(Direction::Left);
                }
                else
                {
                    GameMap[i]->CloseDoor(Direction::Left);
                }

                if (LoadedGame->GetRight(i) == true)
                {
                    GameMap[i]->OpenDoor(Direction::Right);
                }
                else
                {
                    GameMap[i]->CloseDoor(Direction::Right);
                }

                if (LoadedGame->GetUp(i) == true)
                {
                    GameMap[i]->OpenDoor(Direction::Up);
                }
                else
                {
                    GameMap[i]->CloseDoor(Direction::Up);
                }

                if (LoadedGame->GetDown(i) == true)
                {
                    GameMap[i]->OpenDoor(Direction::Down);
                }
                else
                {
                    GameMap[i]->CloseDoor(Direction::Down);
                }

                GameMap[i]->SetisKnown(LoadedGame->GetisKnown(i));
                
            }
            GameControl->SetDay(LoadedGame->GetDay());
            UE_LOG(LogTemp, Warning, TEXT("Saved Day is :%d %d"),GameControl->GetDay(), LoadedGame->GetDay());
            GameControl->SetProgress(LoadedGame->GetProgress());
            currentProgress = GameControl->GetProgress();
            UE_LOG(LogTemp, Warning, TEXT("LOADED"));
        }
        GameControl->SetisLoaded(true);
    }
    UE_LOG(LogTemp, Warning, TEXT("TRYING TO SAVE"));
    if (UShelterGameSave* SaveGameInstance = Cast<UShelterGameSave>(UGameplayStatics::CreateSaveGameObject(UShelterGameSave::StaticClass())))
    {
        // Set data on the savegame object.
        for (int i = 0; i < 100; i++)
        {
            Resource check = GameMap[i]->GetResources();
            bool _Left;
            bool _Up;
            bool _Right;
            bool _Down;
            if (GameMap[i]->GetDoor(Direction::Up) != nullptr && GameMap[i]->GetDoor(Direction::Up)->Status() == DoorStatus::Open)
            {
                _Up = true;
            }
            else
            {
                _Up = false;
            }

            if (GameMap[i]->GetDoor(Direction::Right) != nullptr && GameMap[i]->GetDoor(Direction::Right)->Status() == DoorStatus::Open)
            {
                _Right = true;
            }
            else
            {
                _Right = false;
            }

            if (GameMap[i]->GetDoor(Direction::Down) != nullptr && GameMap[i]->GetDoor(Direction::Down)->Status() == DoorStatus::Open)
            {
                _Down = true;
            }
            else
            {
                _Down = false;
            }

            if (GameMap[i]->GetDoor(Direction::Left) != nullptr && GameMap[i]->GetDoor(Direction::Left)->Status() == DoorStatus::Open)
            {
                _Left = true;
            }
            else
            {
                _Left = false;
            }
            SaveGameInstance->SetRoomData(i, check.food, check.water, check.electricity, _Left, _Right, _Up, _Down, GameMap[i]->isDiscovered());
        }
        for (int i = 0; i < 12; i++)
        {
            SaveGameInstance->SetCCTVData(i, CCTVRoomNum[i]);
        }
        
        UE_LOG(LogTemp, Warning, TEXT("Day was %d. Progress was %d"), GameControl->GetDay(), GameControl->GetProgress());
        SaveGameInstance->SetDayProgress(GameControl->GetDay(), currentProgress);

        GameControl->SetDay(GameControl->GetDay() + 1);

        UE_LOG(LogTemp, Warning, TEXT("Current Day is %d. Have a nice day"), GameControl->GetDay());
        //this condition should be later changed to check if player get the progress item
        GameControl->SetProgress(currentProgress);
        

        // Start async save process.
        UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, TEXT("SAVE"), 0);
    }
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
    UShelterGameSave* LoadedGame = Cast<UShelterGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("SAVE"), 0));
    if (!(GameControl->CheckLoaded()) && LoadedGame != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Load Sequence for CCTV Starts"));
        for (int i = 0; i < 12; ++i)
        {
            ZapPlanes.Add(_ZapPlanes[i]);
            int32 input_num = LoadedGame->GetCCTVData(i);
            CCTVRoomNum.Add(input_num);
            GameControl->SetCCTVData(i, input_num);
        }
    }
    else if (GameControl->CheckCCTV() == false)
    {
        for (int i = 0; i < 12; ++i)
        {
            ZapPlanes.Add(_ZapPlanes[i]);
            int32 input_num = rand() % (maxWidth * maxHeight);

            while (MyContains(input_num))
            {
                input_num = rand() % (maxWidth * maxHeight);
            }
            CCTVRoomNum.Add(input_num);
            GameControl->SetCCTVData(i, input_num);
        }
    }
    else
    {
        for (int i = 0; i < 12; ++i)
        {
            ZapPlanes.Add(_ZapPlanes[i]);
            int32 input_num = GameControl->GetCCTVData(i);
            CCTVRoomNum.Add(input_num);
        }
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
    return Direction::None;
}

void ALevelControl::EndTurn()
{
    // Move monsters in map
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        TMap<Direction, int32> weights = {{Direction::Left, 6},
                                          {Direction::Right, 6},
                                          {Direction::Up, 6},
                                          {Direction::Down, 6},
                                          {Direction::None, 1}};

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

}

void ALevelControl::InitGame(const unsigned int m, const unsigned int n, FString _LevelString)
{
    maxHeight = m;
    maxWidth = n;

    eventFlag.Add("DefaultEvent", false);
    if (GameControl->CheckGenerated() == false)
    {
        GameControl->SetIsGenerated(true);
        UE_LOG(LogTemp, Warning, TEXT("There's nothing yet. Initiate Room Generate Procedure"));
        this->InitRooms();
        for (unsigned int i = 0; i < m*n; i++)
        {
            GameControl->SetGameMapData(i, GameMap[i]);
        }
    }
    else
    {
        GameMap.SetNum(m*n);
        for (unsigned int i = 0; i < m*n; i++)
        {
            GameMap[i] = GameControl->GetGameMapData(i);
        }
        UE_LOG(LogTemp, Warning, TEXT("RoomData Found."));
        currentProgress = GameControl->GetProgress();
    }
    
    this->InitMap(_LevelString);
}

void ALevelControl::InitRooms()
{
    int size = maxHeight * maxWidth;

    for (int i = 0; i < size; i++)
    {
        URoom *newRoom = NewObject<URoom>();
        newRoom->InitRoom(i);
        GameMap.Add(newRoom);

        // A idx is MaxWidth * x + y
        int x = i / maxWidth;
        int y = i % maxWidth;

        if (x != 0)
        {
            URoom *connectedRoom = FindRoomByLocation(x - 1, y);
          
            ADoor *door = NewObject<ADoor>();
            door->InitDoor(newRoom, connectedRoom, DoorStatus::Open);

            newRoom->SetDoor(Direction::Up, door);
            connectedRoom->SetDoor(Direction::Down, door);
        }

        if (y != 0)
        {
            URoom *connectedRoom = FindRoomByLocation(x, y - 1);

            ADoor *door = NewObject<ADoor>();
            door->InitDoor(newRoom, connectedRoom, DoorStatus::Open);

            newRoom->SetDoor(Direction::Left, door);
            connectedRoom->SetDoor(Direction::Right, door);
        }
        GameMap[i]->InitResources(RoomType::None);
    }

    int maxRoomNum = maxWidth * maxHeight;
    // Set random resources
    for (int i = 0; i < maxFoodRoom; ++i)
    {
        int roomNum = rand() % maxRoomNum;
        while (foodRoomNum.Contains(roomNum))
        {
            roomNum = rand() % maxRoomNum;
        }
        if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) +
                abs((int)(roomNum % maxWidth - panicRoomId % maxWidth)) >=
            resourceRoomDistance)
        {
            GameMap[roomNum]->InitResources(RoomType::Food);
            foodRoomNum.Add(roomNum);
        }
    }

    for (int i = 0; i < maxWaterRoom; ++i)
    {
        int roomNum = rand() % maxRoomNum;
        while (foodRoomNum.Contains(roomNum) || waterRoomNum.Contains(roomNum))
        {
            roomNum = rand() % maxRoomNum;
        }
        if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) +
                abs((int)(roomNum % maxWidth - panicRoomId % maxWidth)) >=
            resourceRoomDistance)
        {
            GameMap[roomNum]->InitResources(RoomType::Water);
            waterRoomNum.Add(roomNum);
        }
    }

    for (int i = 0; i < maxElectricityRoom; ++i)
    {
        int roomNum = rand() % maxRoomNum;
        while (foodRoomNum.Contains(roomNum) || waterRoomNum.Contains(roomNum) || electricityRoomNum.Contains(roomNum))
        {
            roomNum = rand() % maxRoomNum;
        }
        if (abs((int)(roomNum / maxWidth - panicRoomId / maxWidth)) +
                abs((int)(roomNum % maxWidth - panicRoomId % maxWidth)) >=
            resourceRoomDistance)
        {
            GameMap[roomNum]->InitResources(RoomType::Electricity);
            electricityRoomNum.Add(roomNum);
        }
    }
	bool progressFlag = false;
	for (unsigned int i = 0; i < maxHeight; ++i) {
		for (unsigned int j = 0; j < maxWidth; ++j) {
			if (!progressFlag && GameMap[i*maxWidth + j]->GetResources().progress) {
				progressFlag = true;
				break;
			}
		}
	}

	if (!progressFlag) {
		int roomNum = rand() % maxRoomNum;
		while (foodRoomNum.Contains(roomNum) || waterRoomNum.Contains(roomNum) || electricityRoomNum.Contains(roomNum))
		{
			roomNum = rand() % maxRoomNum;
		}
		GameMap[roomNum]->SetProgress(true);
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

                // 1 means closed door
                if (ParsedWords[i].Equals("1"))
                {
                    GameMap[currentRoomId]->CloseDoor(Direction::Up);
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

                // 1 means closed door
                if (ParsedWords[i].Equals("1"))
                {
                    GameMap[currentRoomId]->CloseDoor(Direction::Left);
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
    SpawnDoorMesh(panicRoomId);
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


    FVector spawnLocationLeft(startX + col * interval - 305.0f, startY + row * interval, startZ);
    if (roomNum == panicRoomId)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attemp to generate Panic Room Door"));
        PanicRoomDoorList.Add(world->SpawnActor<APanicRoomDoor>(PanicRoomDoor, spawnLocationLeft, rotatorLeft, spawnParams));
    }
    else if (GameMap[roomNum]->GetDoor(Direction::Left) != nullptr &&
        GameMap[roomNum]->GetDoor(Direction::Left)->Status() == DoorStatus::Open)
    {
        world->SpawnActor<ADoor>(DoorActor[0], spawnLocationLeft, rotatorLeft, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoor>(DoorActor[1], spawnLocationLeft, rotatorLeft, spawnParams);
    }


    FVector spawnLocationUp(startX + col * interval, startY + row * interval - 305.0f, startZ);
    if (roomNum == panicRoomId)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attemp to generate Panic Room Door"));
        PanicRoomDoorList.Add(world->SpawnActor<APanicRoomDoor>(PanicRoomDoor, spawnLocationUp, rotatorUp, spawnParams));
    }
    else if (GameMap[roomNum]->GetDoor(Direction::Up) != nullptr &&
        GameMap[roomNum]->GetDoor(Direction::Up)->Status() == DoorStatus::Open)
    {
        world->SpawnActor<ADoor>(DoorActor[0], spawnLocationUp, rotatorUp, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoor>(DoorActor[1], spawnLocationUp, rotatorUp, spawnParams);
    }


    FVector spawnLocationRight(startX + col * interval + 305.0f, startY + row * interval, startZ);
    if (roomNum == panicRoomId)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attemp to generate Panic Room Door"));
        PanicRoomDoorList.Add(world->SpawnActor<APanicRoomDoor>(PanicRoomDoor, spawnLocationRight, rotatorLeft, spawnParams));
		DoorSwitch(Direction::Up);
    }
    else if (GameMap[roomNum]->GetDoor(Direction::Right) != nullptr &&
        GameMap[roomNum]->GetDoor(Direction::Right)->Status() == DoorStatus::Open)
    {
        world->SpawnActor<ADoor>(DoorActor[0], spawnLocationRight, rotatorLeft, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoor>(DoorActor[1], spawnLocationRight, rotatorLeft, spawnParams);
    }

    FVector spawnLocationDown(startX + col * interval, startY + row * interval + 305.0f, startZ);
    if (roomNum == panicRoomId)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attemp to generate Panic Room Door"));
        PanicRoomDoorList.Add(world->SpawnActor<APanicRoomDoor>(PanicRoomDoor, spawnLocationDown, rotatorUp, spawnParams));
    }
    else if (GameMap[roomNum]->GetDoor(Direction::Down) != nullptr &&
        GameMap[roomNum]->GetDoor(Direction::Down)->Status() == DoorStatus::Open)
    {

        world->SpawnActor<ADoor>(DoorActor[0], spawnLocationDown, rotatorUp, spawnParams);
    }
    else
    {
        world->SpawnActor<ADoor>(DoorActor[1], spawnLocationDown, rotatorUp, spawnParams);
    }
}

// calculate and add visible room numbers
void ALevelControl::InitVisibleRoom()
{

    // Left side visible rooms
    URoom *currentRoom = GameMap[panicRoomId - 1];
    ADoor *currentDoor = currentRoom->GetDoor(Direction::Left);
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
        if (currentRoom == nullptr || currentDoor == nullptr || currentDoor->Status() == DoorStatus::Close)
        {
            break;
        }

        currentRoom = currentRoom->BeyondDoor(Direction::Left);
        currentDoor = currentRoom->GetDoor(Direction::Left);

        currentRoomNum--;
    }

    currentRoom = GameMap[panicRoomId + 10];
    currentDoor = currentRoom->GetDoor(Direction::Down);
    currentRoomNum = panicRoomId + 10;

    while (1)
    {

        // spawn room if cctv has not spawned room for current room
        if (!CCTVRoomNum.Contains(currentRoomNum))
        {
            SpawnRoomMesh(currentRoomNum);
            VisibleRoomNum.Add(currentRoomNum);
        }
        if (currentRoom == nullptr || currentDoor == nullptr || currentDoor->Status() == DoorStatus::Close)
        {
            break;
        }

        currentRoom = currentRoom->BeyondDoor(Direction::Down);
        currentDoor = currentRoom->GetDoor(Direction::Down);

        currentRoomNum += 10;
    }

    currentRoom = GameMap[panicRoomId + 1];
    currentDoor = currentRoom->GetDoor(Direction::Right);
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
        if (currentRoom == nullptr || currentDoor == nullptr || currentDoor->Status() == DoorStatus::Close)
        {
            break;
        }

        currentRoom = currentRoom->BeyondDoor(Direction::Right);
        currentDoor = currentRoom->GetDoor(Direction::Right);

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
                monsterActors[_monsterId - 1]->MoveTo(FVector(startX + interval * (panicRoomId % maxWidth),
                                                              startY + interval * (panicRoomId / maxWidth), startZ));
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

void ALevelControl::UseElectricity()
{
	if (IsElectricityEnough()) {
		URoom* panicRoom = GameMap[panicRoomId];
		panicRoom->SetElectricity(GameMap[panicRoomId]->GetResources().electricity - electricityUsage * electricityDecreaseSpeed);
		if (IsElectricityZero()) {
			PowerDownEvent.Broadcast();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("not enough electricity!"))
	}
	
	return;
}

bool ALevelControl::IsElectricityZero()
{
	if (GameMap[panicRoomId]->GetResources().electricity == 0) {
		return true;
	}
	return false;
}

void ALevelControl::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("GameOver"))
	
	IsGameOver = true;
	GameOverEvent.Broadcast();
	return;
}

float ALevelControl::GetElectricityPercent()
{
    float p = GameMap[panicRoomId]->GetResources().electricity;
    return p / (float)maxElectricity * 100.0f;
}

void ALevelControl::DoorSwitch(Direction d)
{
	GameMap[panicRoomId]->SwitchDoor(d);
	PanicRoomDoorList[(uint8)d]->SetDoor();

	return;
}

bool ALevelControl::DoorSwitchByUser(Direction d)
{
	if (GameMap[panicRoomId]->GetResources().electricity < electricityDoorInstantUsage) {
		return false;
	}

    GameMap[panicRoomId]->SwitchDoor(d);
    PanicRoomDoorList[(uint8)d]->SetDoor();

	GameMap[panicRoomId]->SetElectricity(GameMap[panicRoomId]->GetResources().electricity - electricityDoorInstantUsage);

	if (IsElectricityZero()) {
		PowerDownEvent.Broadcast();
	}

	if (GameMap[panicRoomId]->GetDoor(d)->Status() == DoorStatus::Close) {
		electricityUsage += electricityDoorUsage;
	}
	return true;
}

void ALevelControl::OpenAllPanicRoomDoors()
{
	if (GameMap[panicRoomId]->GetDoor(Direction::Right)->Status() == DoorStatus::Close) {
		GameMap[panicRoomId]->SwitchDoor(Direction::Right);
		PanicRoomDoorList[(uint8)Direction::Right]->SetDoor();
	}
	
	if (GameMap[panicRoomId]->GetDoor(Direction::Down)->Status() == DoorStatus::Close) {
		GameMap[panicRoomId]->SwitchDoor(Direction::Down);
		PanicRoomDoorList[(uint8)Direction::Down]->SetDoor();
	}

	if (GameMap[panicRoomId]->GetDoor(Direction::Left)->Status() == DoorStatus::Close) {
		GameMap[panicRoomId]->SwitchDoor(Direction::Left);
		PanicRoomDoorList[(uint8)Direction::Left]->SetDoor();
	}
}

int ALevelControl::GetPanicRoomFood()
{
	return GameMap[panicRoomId]->GetResources().food;
}

int ALevelControl::GetPanicRoomWater()
{
	return GameMap[panicRoomId]->GetResources().water;
}

int ALevelControl::GetFoodComplete()
{
	return foodComplete;
}

int ALevelControl::GetWaterComplete()
{
	return waterComplete;
}

float ALevelControl::GetElectricityComplete()
{
	return electricityComplete;
}

void ALevelControl::EndLevelPreparation()
{
    //resources used during day
    Resource panicRoomLeft = GameMap[panicRoomId]->GetResources();
    GameMap[panicRoomId]->SetFood(panicRoomLeft.food - ResourcesUsed);
    GameMap[panicRoomId]->SetWater(panicRoomLeft.water - ResourcesUsed);
    GameMap[panicRoomId]->SetElectricity(panicRoomLeft.electricity - (float)ResourcesUsed);

    GameMap[panicRoomId]->OpenDoor(Direction::Up);
    GameMap[panicRoomId]->OpenDoor(Direction::Down);
    GameMap[panicRoomId]->OpenDoor(Direction::Right);
    GameMap[panicRoomId]->OpenDoor(Direction::Left);
}

void ALevelControl::SetPanicRoomFood(int _value)
{
	GameMap[panicRoomId]->SetFood(_value);
}

void ALevelControl::SetPanicRoomWater(int _value)
{
	GameMap[panicRoomId]->SetWater(_value);
}

// Returns resource type, resource size. resource type = 0 for not discovered or not known
TArray<FResourceUI> ALevelControl::GetRoomResourceUI()
{
    TArray<FResourceUI> ResourceArray;
    URoom *currentRoom;

    int maxRoomNum = maxWidth * maxHeight;
    for (int i = 0; i < maxRoomNum; ++i)
    {
        currentRoom = GameMap[i];
        FResourceUI currentResourceUI;

        if (currentRoom->isDiscovered())
        {

            // find maximum resource type and its size
            Resource currentResource = currentRoom->GetResources();
			if (currentResource.progress) {
				currentResourceUI.resourceType = 4;
				currentResourceUI.resourceSize = 1;
			}
            else if (currentResource.food >= currentResource.water)
            {
                if (currentResource.food >= currentResource.electricity)
                {
                    currentResourceUI.resourceType = 1;
                    currentResourceUI.resourceSize = currentResource.food;
                }
                else
                {
                    currentResourceUI.resourceType = 3;
                    currentResourceUI.resourceSize = currentResource.electricity;
                }
            }
            else if (currentResource.water >= currentResource.electricity)
            {
                currentResourceUI.resourceType = 2;
                currentResourceUI.resourceSize = currentResource.water;
            }
            else
            {
                currentResourceUI.resourceType = 3;
                currentResourceUI.resourceSize = currentResource.electricity;
            }
        }
        else
        {
            // undiscovered room
            currentResourceUI.resourceType = 0;
            currentResourceUI.resourceSize = 0;
        }
        ResourceArray.Add(currentResourceUI);
    }

    return ResourceArray;
}

TArray<DoorStatus> ALevelControl::GetDoorUI()
{
    TArray<DoorStatus> DoorArray;
    int currentRoomNum;
    for (unsigned int i = 0; i < maxHeight; ++i)
    {
        for (unsigned int j = 0; j < maxWidth; ++j)
        {
            currentRoomNum = i * maxWidth + j;
            if (j == 0)
            {
                DoorArray.Add(DoorStatus::Close);
            }
            else
            {
                DoorArray.Add(GameMap[currentRoomNum]->GetDoor(Direction::Left)->Status());
            }

            if (i == 0)
            {
                DoorArray.Add(DoorStatus::Close);
            }
            else
            {
                DoorArray.Add(GameMap[currentRoomNum]->GetDoor(Direction::Up)->Status());
            }
        }
    }
    return DoorArray;
}

int ALevelControl::GetProgressUI()
{
	return currentProgress;
}

int ALevelControl::GetMaxProgressUI()
{
    return GameControl->GetMaxProgress();
}

bool ALevelControl::IsBlocked(int _monsterId)
{
    ADoor *door = nullptr;
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (it.Key == _monsterId)
        {
            int roomNumber = it.Value;

            if (roomNumber == panicRoomId - 1)
            {
                door = GameMap[panicRoomId]->GetDoor(Direction::Left);
            }
            else if (roomNumber == panicRoomId + 1)
            {
                door = GameMap[panicRoomId]->GetDoor(Direction::Right);
            }
            else if (roomNumber == panicRoomId + 10)
            {
                door = GameMap[panicRoomId]->GetDoor(Direction::Down);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("monster %d roomNumber error!"), _monsterId);
                return true;
            }
            break;
        }
    }
    if (door != nullptr && door->Status() == DoorStatus::Close)
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

    int idx = static_cast<int>(monsterType);

    if (MonsterSpawn[idx])
    {
        UWorld *world = GetWorld();
        if (world)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = this;

            FRotator rotator(0.0f, 0.0f, 0.0f);
            FVector spawnLocation(startX + col * interval, startY + row * interval, startZ);

            monsterActors.Add(world->SpawnActor<AMonster>(MonsterSpawn[idx], spawnLocation, rotator, spawnParams));
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
    PrintMap();
    for (const TPair<int32, int32> &it : monsterLocations)
    {
        if (it.Key == monsterId)
        {
            if (d == Direction::None)
            {
                monsters[it.Key]->PreviousDirection(Direction::None);
                return true;
            }
			if (monsterActors[monsterId - 1]->IsAngry || !(monsterActors[monsterId - 1]->IsReadyToMove()))
            {
                continue;
            }

            ADoor *door = GameMap[it.Value]->GetDoor(d);

            if (door == nullptr || door->Status() == DoorStatus::Close ||
                GameMap[it.Value]->BeyondDoor(d)->MonsterId() != 0 ||
                GameMap[it.Value]->BeyondDoor(d)->RoomId() == panicRoomId)
            {
                return false;
            }

            // when monster successfully moving to next room
            GameMap[it.Value]->DeleteMonster();
            monsterLocations[it.Key] = GameMap[it.Value]->BeyondDoor(d)->RoomId();
            GameMap[it.Value]->InsertMonster(monsterId);

            // move matching monster to next room
            if (monsterActors[monsterId - 1])
            {
                // monsterActors[monsterId - 1]->SetActorLocation(FVector(startX + interval * (it.Value % maxWidth),
                // startY + interval * (it.Value / maxWidth), startZ));
                monsterActors[monsterId - 1]->MoveTo(FVector(startX + interval * (it.Value % maxWidth),
                                                             startY + interval * (it.Value / maxWidth), startZ));
            }

            Direction prev = OppositeDirection(d);
            monsters[it.Key]->PreviousDirection(prev);
            break;
        }
    }
    return true;
}

bool ALevelControl::IsElectricityEnough()
{
	if (GameMap[panicRoomId]->GetResources().electricity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("enough with %f"), GameMap[panicRoomId]->GetResources().electricity)
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("not enough with %f"), GameMap[panicRoomId]->GetResources().electricity)
		return false;
	}
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

float ALevelControl::ResourceCheckByRobot(int RoomId, int Type)
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

bool ALevelControl::CircuitCheckByRobot(int RoomId)
{
	if (GameMap[RoomId]->GetResources().progress) {
		return true;
	}
	else {
		return false;
	}
}

void ALevelControl::RemoveCircuit(int RoomId)
{
	GameMap[RoomId]->SetProgress(false);
}

void ALevelControl::AddProgress()
{
	currentProgress++;
    GameControl->SetProgress(currentProgress);
}

void ALevelControl::SetRoomResources(int RoomId, int food, int water, float electricity)
{

    UE_LOG(LogTemp, Warning, TEXT("elect before function is : %f"), electricity);
	if (electricity > maxElectricity) {
		electricity = maxElectricity;
	}
	if (food > maxFood) {
		food = maxFood;
	}
	if (water > maxWater) {
		water = maxWater;
	}
    GameMap[RoomId]->SetResources(food, water, electricity);
    UE_LOG(LogTemp, Warning, TEXT("elect after function is : %f"), electricity);
}

void ALevelControl::RobotCheck(int RoomId)
{
    GameMap[RoomId]->SetisKnown(true);
}

bool ALevelControl::IsRoomClosed(int roomNum,
                                 int direction) // For RobotControl Usage. 1 = up, 2 = right, 3 = down, 4 = left
{

    ADoor *door = nullptr;
    if (direction == 1)
    {
        door = GameMap[roomNum]->GetDoor(Direction::Up);
    }
    else if (direction == 2)
    {
        door = GameMap[roomNum]->GetDoor(Direction::Right);
    }
    else if (direction == 3)
    {
        door = GameMap[roomNum]->GetDoor(Direction::Down);
    }
    else if (direction == 4)
    {
        door = GameMap[roomNum]->GetDoor(Direction::Left);
    }
    if (door == nullptr || door->Status() == DoorStatus::Close)
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
