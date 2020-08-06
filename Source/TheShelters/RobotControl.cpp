// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotControl.h"

// Sets default values
ARobotControl::ARobotControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobotControl::BeginPlay()
{
	Super::BeginPlay();
    initMap();

    FRotator rotator(0.0f, 0.0f, 0.0f);
    FVector spawnLocation(11000, 0, 200);
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    Robot = GetWorld()->SpawnActor<ARobotPawn>(RobotSpawn, spawnLocation, rotator, spawnParams);
    Robot->SetRobotControl(this);
    UE_LOG(LogTemp, Warning, TEXT("Spawned"));
}    

void ARobotControl::initMap()
{
    currentLocation = startLocation;
    visited.Init(0, 100);
    route.RemoveAll([](int val) { return true; });
    route.Add(startLocation);
    visited[startLocation] = 1;
    ToDestination = true;
    CurrentIndex = 0;
    MonsterCheck = false;
}

// Called every frame
void ARobotControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (MonsterCheck)
    {
        int length = Monsters.Num();
        for (int i = 0; i < length; i++)
        {
            FVector diff = Robot->GetActorLocation() - Monsters[i]->GetActorLocation();
            float dist = sqrt(diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z);
            if (dist < 1000 && ToDestination == true)
            {
                UE_LOG(LogTemp, Warning, TEXT("moving back"));
                ToDestination = false;
                StartMoving();
            }
        }
    }
}

void ARobotControl::GiveAddress(TArray<ASurvivor*> _List)
{
    _List[0]->InitRobots(this);
}

void ARobotControl::GetMonsters(TArray<AMonster*> _Monsters)
{
    int length = _Monsters.Num();
    for (int i = 0; i < length; i++)
    {
        Monsters.Add(_Monsters[i]);
    }
}

void ARobotControl::MapLeft()
{
    if (RoomControl->IsRoomClosed(currentLocation, 2))
    {
        if (currentLocation % 10 != 9)
        {
            if (visited[currentLocation + 1] == 0)
            {
                currentLocation += 1;
                visited[currentLocation] = 1;
                route.Add(currentLocation);
            }
            else
            {
                if (route[route.Num() - 2] == (currentLocation + 1))
                {
                    UE_LOG(LogTemp, Warning, TEXT("going backward"));
                    visited[currentLocation] = 0;
                    currentLocation += 1;
                    route.Pop();
                }
            }
        }
    }
    PrintMap();
}

void ARobotControl::MapRight()
{
    if (RoomControl->IsRoomClosed(currentLocation, 4))
    {
        if (currentLocation % 10 != 0)
        {
            if (visited[currentLocation - 1] == 0)
            {
                currentLocation -= 1;
                visited[currentLocation] = 1;
                route.Add(currentLocation);
            }
            else
            {
                if (route[route.Num() - 2] == (currentLocation - 1))
                {
                    UE_LOG(LogTemp, Warning, TEXT("going backward"));
                    visited[currentLocation] = 0;
                    currentLocation -= 1;
                    route.Pop();
                }
            }
        }
    }
    PrintMap();
}

void ARobotControl::MapDown()
{
    if (RoomControl->IsRoomClosed(currentLocation, 1))
    {
        if (currentLocation / 10 != 0)
        {
            if (visited[currentLocation - 10] == 0)
            {
                currentLocation -= 10;
                visited[currentLocation] = 1;
                route.Add(currentLocation);
            }
            else //already visited
            {
                if (route[route.Num() - 2] == (currentLocation - 10))
                {
                    UE_LOG(LogTemp, Warning, TEXT("going backward"));
                    visited[currentLocation] = 0;
                    currentLocation -= 10;
                    route.Pop();
                }
            }
        }
    }
    PrintMap();
}

void ARobotControl::MapUp()
{
    if (RoomControl->IsRoomClosed(currentLocation, 3))
    {
        if (currentLocation / 10 != 9)
        {
            if (visited[currentLocation + 10] == 0)
            {
                currentLocation += 10;
                visited[currentLocation] = 1;
                route.Add(currentLocation);
            }
            else
            {
                if (route[route.Num() - 2] == (currentLocation + 10))
                {
                    UE_LOG(LogTemp, Warning, TEXT("going backward"));
                    visited[currentLocation] = 0;
                    currentLocation += 10;
                    route.Pop();
                }
            }
        }
    }
    PrintMap();
}

void ARobotControl::SetMove()
{
    if (isMoving == false)
    {
        isMoving = true;
        Robot->SetMovement(true);
        MonsterCheck = true;
        StartMoving();
    }
}

bool ARobotControl::StartMoving()
{
    if (isMoving)
    {
        int LengthOfRoute = route.Num();
        if (LengthOfRoute > 1)
        {
            if (ToDestination)
            {
                CurrentIndex++;
                RobotMoveTo(route[CurrentIndex]);
                ResourceSearch(route[CurrentIndex]);
                if (route[CurrentIndex] == route[LengthOfRoute - 1])
                {
                    ReachDestination();
                    ToDestination = false;
                }
                
            }
            else if (!ToDestination)
            {
                if (route[CurrentIndex] == route[0])
                {
                    EndMovement();
                    return true;
                }
                Robot->SetArrival(false);
                CurrentIndex--;
                if (CurrentIndex < 0)
                {
                    CurrentIndex = 0;
                }
                RobotMoveTo(route[CurrentIndex]);
            }
        }
    }
    
    return true;
}

void ARobotControl::ResourceSearch(int RoomId)
{
    RoomResources.food = RoomControl->ResourceCheckByRobot(RoomId,1);
    RoomResources.water = RoomControl->ResourceCheckByRobot(RoomId, 2);
    RoomResources.electricity = RoomControl->ResourceCheckByRobot(RoomId, 3);


    UE_LOG(LogTemp, Warning, TEXT("Food: %d Water: %d Elect: %d"), RoomResources.food, RoomResources.water, RoomResources.electricity);
    SearchData.Add(RoomId);
}

//make RobotActor to move to RoomIndex Room
void ARobotControl::RobotMoveTo(int RoomIndex)
{
    int x = RoomIndex % 10;
    int y = RoomIndex / 10;
    Robot->SetDestination(FVector(startX + x * interval, startY + y * interval, startZ));
    //Robot->SetActorLocation(FVector(startX + x * interval, startY + y * interval, startZ));
}

//check resources at the destination index room. Start recall function
void ARobotControl::ReachDestination()
{
    LoadedResources.food = RoomResources.food;
    LoadedResources.water = RoomResources.water;
    LoadedResources.electricity = RoomResources.electricity;
    UE_LOG(LogTemp, Warning, TEXT("Reach Destination. Gain Food: %d Water: %d Elect: %d"), RoomResources.food, RoomResources.water, RoomResources.electricity);
    RoomControl->SetRoomResources(route[CurrentIndex],0, 0, 0);
    Robot->SetArrival(true);
}

//going back to panicRoom
void ARobotControl::EndMovement()
{
    PanicRoomResources.food = RoomControl->ResourceCheckByRobot(startLocation, 1);
    PanicRoomResources.water = RoomControl->ResourceCheckByRobot(startLocation, 2);
    PanicRoomResources.electricity = RoomControl->ResourceCheckByRobot(startLocation, 3);
    PanicRoomResources.food += LoadedResources.food;
    PanicRoomResources.water += LoadedResources.water;
    PanicRoomResources.electricity += LoadedResources.electricity;

    UE_LOG(LogTemp, Warning, TEXT("Resource status(Food: %d Water: %d Elect: %d"), PanicRoomResources.food, PanicRoomResources.water, PanicRoomResources.electricity);

    RoomControl->SetRoomResources(startLocation, PanicRoomResources.food, PanicRoomResources.water, PanicRoomResources.electricity);

    Robot->SetMovement(false);
    isMoving = false;
    initMap();
    
    int searchLength = SearchData.Num();
    for (int i = 0; i < searchLength; i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("SearchData from %d room"), SearchData[i]);
        RoomControl->RobotCheck(SearchData[i]);
    }
    SearchData.Empty();
}
void ARobotControl::PrintMap()
{
    FString rou = FString();
    for (int x = 9; x >= 0; x--)
    {
        FString line = FString();
        for (int y = 9; y >= 0; y--)
        {
            int idx = x * 10 + y;

            if (visited[idx] == 0)
            {
                line += TEXT("0 ");
            }
            else
            {
                line += TEXT("1 ");
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("%s"), *line);
    }
    for (int i = 0; i < route.Num(); i++)
    {
        TArray<FStringFormatArg> args;
        args.Add(FStringFormatArg(route[i]));
        rou += FString::Format(TEXT("{0} "), args);
    }
    UE_LOG(LogTemp, Warning, TEXT("%s"), *rou);
}

void ARobotControl::FindRoomControl(TArray<ARoomControl*> _RoomControl)
{
    RoomControl = _RoomControl[0];
}

void ARobotControl::DetectMonster()
{
    ToDestination = false;
}
