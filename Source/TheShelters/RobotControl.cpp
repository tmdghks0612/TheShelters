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
    FVector spawnLocation(9000, 0, 200);
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

void ARobotControl::GetMonsters(TArray<AMonsterActor*> _Monsters)
{
    int length = _Monsters.Num();
    for (int i = 0; i < length; i++)
    {
        Monsters.Add(_Monsters[i]);
    }
}

void ARobotControl::MapRight()
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

void ARobotControl::MapLeft()
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

void ARobotControl::MapUp()
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

void ARobotControl::MapDown()
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
    isMoving = true;
    Robot->SetMovement(true);
    MonsterCheck = true;
    StartMoving();
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
                if (route[CurrentIndex] == route[LengthOfRoute - 1])
                {
                    ReachDestination();
                    ToDestination = false;
                }
            }
            else if (!ToDestination)
            {
                Robot->SetArrival(false);
                CurrentIndex--;
                if (CurrentIndex < 0)
                {
                    CurrentIndex = 0;
                }
                RobotMoveTo(route[CurrentIndex]);
                if (route[CurrentIndex] == route[0])
                {
                    EndMovement();
                }
            }
        }
    }
    
    return true;
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
    UE_LOG(LogTemp, Warning, TEXT("Arrival"));
    Robot->SetArrival(true);
}

//going back to panicRoom
void ARobotControl::EndMovement()
{
    Robot->SetMovement(false);
    isMoving = false;
    initMap();
    UE_LOG(LogTemp, Warning, TEXT("Return"));
}
void ARobotControl::PrintMap()
{
    FString rou = FString();
    for (unsigned int x = 0; x < 10; x++)
    {
        FString line = FString();
        for (unsigned int y = 0; y < 10; y++)
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
