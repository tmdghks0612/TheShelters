// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelter.h"

//Default Constructor
UShelter::UShelter() {}

// Constructor
void UShelter::InitShelter(int num)
{
	monsterId = 0;
	shelterType = Empty;
	shelterId = num;
	cctv = false;
	shelterStatus = Peaceful;
	panicRoomConnection = false;

	doors = std::map<Direction, Door>();
	doors[Left] = { 0, Open };
	doors[Right] = { 0, Open };
	doors[Up] = { 0, Open };
	doors[Down] = { 0, Open };
}

void UShelter::InitDoor(const Direction d, int _shelterId, DoorStatus s) {
	doors[d].connectedShelter = _shelterId;
	doors[d].status = s;
}

// Getters and Setters
const int  UShelter::ShelterId()				const { return shelterId; }
const int  UShelter::MonsterId()				const { return monsterId; }
const bool UShelter::IsConnectedToPanicRoom()   const { return panicRoomConnection; }
const Door UShelter::GetDoor(const Direction d)		  { return doors[d]; }

void UShelter::SetDoor(const Direction d, const DoorStatus status) { doors[d].status = status; }

void UShelter::InsertMonster(int newMonsterId) { monsterId = newMonsterId; }
void UShelter::DeleteMonster()				   { monsterId = 0;}

void UShelter::Radiated()
{
	UE_LOG(LogTemp, Warning, TEXT("radiated"));
}
