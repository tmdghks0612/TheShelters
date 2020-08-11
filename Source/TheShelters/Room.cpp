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
    /*ADoor* temp = nullptr;
    ADoor* temp1 = nullptr;
    ADoor* temp2 = nullptr;
    ADoor* temp3 = nullptr;
    */
    doors.Add(nullptr);
    doors.Add(nullptr);
    doors.Add(nullptr);
    doors.Add(nullptr);
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
    //FILE* f;
    //f = fopen("D:\\trial.txt", "a+");
    //fprintf(f, "RoomId: %d Direction: %d ", roomId, d);
    //fclose(f);
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

void URoom::SwitchDoor(const Direction d)
{
    if (doors[d])
        doors[d]->SwitchStatus();
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

void URoom::SetResources(unsigned int _food, unsigned int _water, float _electricity)
{
    UE_LOG(LogTemp, Warning, TEXT("SetResources"));
    resources.food = _food;
    resources.water = _water;
    resources.electricity = _electricity;
}

void URoom::SetFood(unsigned int _food)
{
	resources.food = _food;
}

void URoom::SetWater(unsigned int _water)
{
	resources.water = _water;
}

void URoom::SetElectricity(float _electricity)
{
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

