// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

//Default Constructor
URoom::URoom() {

}

// Constructor
void URoom::InitRoom(int no)
{
	monster = 0;
	roomType = Empty;
	roomNo = no;
	cctv = false;
	storage = 0;
	roomStatus = Peaceful;
	panicRoomConnection = false;

	doors = std::map<Direction, Door>();
	doors[Left] = { 0, Open };
	doors[Right] = { 0, Open };
	doors[Up] = { 0, Open };
	doors[Down] = { 0, Open };

}

// Public Methods
int URoom::GetRoomNo() {
	return roomNo;
}

void URoom::SetLocation(const unsigned int x, const unsigned int y) {
	location = { x, y };
}

Location URoom::GetLocation() {
	return location;
}

bool URoom::IsConnectedToPanicRoom() {
	return panicRoomConnection;
}

Door* URoom::GetDoor(const Direction d) {
	UE_LOG(LogTemp, Warning, TEXT("Door Status : %d"), d);
	//UE_LOG(LogTemp, Warning, TEXT("Door Status : %d"), doors[d].Status);
	//return &doors.at(d);
	
	//return &doors[d];
	return nullptr;
}

void URoom::SetDoor(const Direction d, URoom* room, DoorStatus s) {
	doors[d].ConnectedRoom = room;
	doors[d].Status = s;
}

void URoom::OpenDoor(const Direction d) {
	doors[d].Status = Open;
}

void URoom::CloseDoor(const Direction d) {
	doors[d].Status = Close;
}

UMonster* URoom::GetMonster() {
	return monster;
}

void URoom::InsertMonster(UMonster* newMonster) {
	this->monster = newMonster;
}

UMonster* URoom::DeleteMonster() {
	UMonster* m = monster;
	monster = 0;
	return m;
}