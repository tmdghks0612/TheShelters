// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"

// Default Constructor
URoom::URoom()
{
}

URoom::~URoom()
{
}

// Constructor
void URoom::InitRoom(int num)
{
    monsterId = 0;
    roomId = num;
    cctv = false;
    roomStatus = Peaceful;
	isKnown = false;

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

Resource URoom::GetResources()
{
	return resources;
}

void URoom::SetResources(unsigned int _food, unsigned int _water, unsigned int _electricity)
{
    UE_LOG(LogTemp, Warning, TEXT("SetResources"));
    resources.food = _food;
    resources.water = _water;
    resources.electricity = _electricity;
}

void URoom::InitResources(RoomType _roomType)
{
	resources.food = rand() % resourceThreshold;
	resources.water = rand() % resourceThreshold;
	resources.electricity = rand() % resourceThreshold;

/*
	// test code
	isKnown = Known;*/
	if (_roomType == Food) {
		// test code
		isKnown = true;
		resources.food += resourceThreshold;
	}
	else if (_roomType == Water) {
		// test code
		isKnown = true;
		resources.water += resourceThreshold;

	}
	else if (_roomType == Electricity) {
		// test code
		isKnown = true;
		resources.electricity += resourceThreshold;
	}
}

bool URoom::isDiscovered()
{
	return isKnown;
}

void URoom::SetisKnown(bool _check)
{
    isKnown = _check;
}

void URoom::Radiated()
{
    UE_LOG(LogTemp, Warning, TEXT("radiated"));
}

