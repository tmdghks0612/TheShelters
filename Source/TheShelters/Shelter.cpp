// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelter.h"

//Default Constructor
UShelter::UShelter() {

}

// Constructor
void UShelter::InitShelter(int num)
{
	monsterId = 0;
	shelterType = Empty;
	shelterNum = num;
	cctv = false;
	storage = 0;
	shelterStatus = Peaceful;
	panicRoomConnection = false;

	doors = std::map<Direction, Door>();
	doors[Left] = { 0, Open };
	doors[Right] = { 0, Open };
	doors[Up] = { 0, Open };
	doors[Down] = { 0, Open };

}

// Public Methods
int UShelter::GetShelterNum() {
	return shelterNum;
}

void UShelter::SetLocation(const unsigned int x, const unsigned int y) {
	location = { x, y };
}

Location UShelter::GetLocation() {
	return location;
}

bool UShelter::IsConnectedToPanicRoom() {
	return panicRoomConnection;
}

Door UShelter::GetDoor(const Direction d) {
	UE_LOG(LogTemp, Warning, TEXT("Door Status : %d"), d);
	
	return doors[d];
}

void UShelter::SetDoor(const Direction d, int _shelterNum, DoorStatus s) {
	doors[d].connectedShelter = _shelterNum;
	doors[d].status = s;
}

void UShelter::OpenDoor(const Direction d) {
	doors[d].status = Open;
}

void UShelter::CloseDoor(const Direction d) {
	doors[d].status = Close;
}

int UShelter::GetMonsterId() {
	return monsterId;
}

void UShelter::InsertMonster(int newMonsterId) {
	monsterId = newMonsterId;
}

void UShelter::DeleteMonster() {
	monsterId = 0;
	return;
}

void UShelter::Radiated()
{
	UE_LOG(LogTemp, Warning, TEXT("radiated"));
}

