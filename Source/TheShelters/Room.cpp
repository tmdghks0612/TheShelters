// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"

// Default Constructor
URoom::URoom()
{
}

// Constructor
void URoom::InitRoom(int num)
{
    monsterId = 0;
    roomType = Empty;
    roomId = num;
    cctv = false;
    roomStatus = Peaceful;

    doors = std::map<Direction, Door>();
    doors[Left] = {nullptr, Open};
    doors[Right] = {nullptr, Open};
    doors[Up] = {nullptr, Open};
    doors[Down] = {nullptr, Open};
}

void URoom::InitDoor(const Direction d, URoom *connectedRoom, DoorStatus s)
{
    doors[d].connectedRoom = connectedRoom;
    doors[d].status = s;
}

// Getters and Setters
const int URoom::RoomId() const
{
    return roomId;
}
const int URoom::MonsterId() const
{
    return monsterId;
}
const Door URoom::GetDoor(const Direction d)
{
    return doors[d];
}

void URoom::SetDoor(const Direction d, const DoorStatus status)
{
    doors[d].status = status;
}
void URoom::OpenDoor(const Direction d)
{
    SetDoor(d, Open);
    Direction oppositeDirection = static_cast<Direction>((d + 2) % 4);
    doors[d].connectedRoom->SetDoor(oppositeDirection, Open);
}
void URoom::CloseDoor(const Direction d)
{
    SetDoor(d, Close);
    Direction oppositeDirection = static_cast<Direction>((d + 2) % 4);
    doors[d].connectedRoom->SetDoor(oppositeDirection, Close);
}

void URoom::InsertMonster(int newMonsterId)
{
    monsterId = newMonsterId;
}
void URoom::DeleteMonster()
{
    monsterId = 0;
}

void URoom::Radiated()
{
    UE_LOG(LogTemp, Warning, TEXT("radiated"));
}
