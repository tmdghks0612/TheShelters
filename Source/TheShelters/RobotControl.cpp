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
    FVector spawnLocation(1110, 150, 250);
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    Robot = GetWorld()->SpawnActor<ARobotActor>(spawnLocation, rotator, spawnParams);
    
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
}

// Called every frame
void ARobotControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARobotControl::RobotRouteSelect(TArray<UObject *> ShelterMap)
{
    for (int i= 0; i<100; i++)
    {
        MapObject.Add(ShelterMap[i]);
    }
}

void ARobotControl::GiveAddress(TArray<ASurvivor*> _List)
{
    _List[0]->InitRobots(this);   
}

void ARobotControl::MapRight()
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
    PrintMap();
}

void ARobotControl::MapLeft()
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
            if (route[route.Num()-2] == (currentLocation - 1))
            {
                UE_LOG(LogTemp, Warning, TEXT("going backward"));
                visited[currentLocation] = 0;
                currentLocation -= 1;
                route.Pop();
            }
        }
    }
    PrintMap();
}

void ARobotControl::MapUp()
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
            if (route[route.Num()-2] == (currentLocation - 10))
            {
                UE_LOG(LogTemp, Warning, TEXT("going backward"));
                visited[currentLocation] = 0;
                currentLocation -= 10;
                route.Pop();
            }
        }
    }
    PrintMap();
}

void ARobotControl::MapDown()
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
    PrintMap();
}

void ARobotControl::StartMoving()
{
    int LengthOfRoute = route.Num();
    if (ToDestination)
    {
        CurrentIndex++;
        RobotMoveTo(route[CurrentIndex]);
        if (route[CurrentIndex] == route[LengthOfRoute])
        {
            ReachDestination();
            ToDestination = false;
        }
    }
    else if(!ToDestination)
    {
        CurrentIndex--;
        RobotMoveTo(route[CurrentIndex]);
        if (route[CurrentIndex] == route[0])
        {
            EndMovement();
        }
    }
    
}

//make RobotActor to move to RoomIndex Room
void ARobotControl::RobotMoveTo(int RoomIndex)
{
    Robot->CheckWorking();

}

//check resources at the destination index room. Start recall function
void ARobotControl::ReachDestination()
{

}

//going back to panicRoom
void ARobotControl::EndMovement()
{

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