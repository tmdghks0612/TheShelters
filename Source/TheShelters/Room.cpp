// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

//Default Constructor
URoom::URoom() {

}

// Constructor
URoom::URoom(int no)
	: monster(0),
	roomType(Empty),
	roomNo(no),
	cctv(false),
	storage(0),
	roomStatus(Peaceful),
	panicRoomConnection(false)
{
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

const Door* URoom::GetDoor(const Direction d) {
	return &doors.at(d);
}

void URoom::SetDoor(const Direction d, URoom* room, DoorStatus s = Open) {
	doors[d].ConnectedRoom = room;
	doors[d].Status = s;
}

void URoom::OpenDoor(const Direction d) {
	doors[d].Status = Open;
}

void URoom::CloseDoor(const Direction d) {
	doors[d].Status = Close;
}

const UMonster* URoom::GetMonster() {
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