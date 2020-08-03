// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "Door.h"

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

    doors.Add(Left, nullptr);
    doors.Add(Right, nullptr);
    doors.Add(Up, nullptr);
    doors.Add(Down, nullptr);
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
ADoor *URoom::GetDoor(const Direction d)
{
    return doors[d];
}
URoom *URoom::BeyondDoor(const Direction d)
{
    return doors[d]->GetBeyond(this);
}

void URoom::SetDoor(const Direction d, ADoor *door)
{
    doors[d] = door;
}
void URoom::OpenDoor(const Direction d)
{
    if (doors[d])
        doors[d]->Open();
}
void URoom::CloseDoor(const Direction d)
{
    if (doors[d])
        doors[d]->Close();
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

void URoom::Radiated()
{
    UE_LOG(LogTemp, Warning, TEXT("radiated"));
}

