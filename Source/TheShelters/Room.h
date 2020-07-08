// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "Monster.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

/**
 * 
 */

class URoom;

enum ResourceType { Empty, Electricity, Water, Food, Metal, Circuit };
enum RoomStatus { Peaceful, Dangerous };
enum DoorStatus { Open, Close };
enum Direction { Left, Right, Up, Down };

typedef struct {
	unsigned int x;
	unsigned int y;
} Location;

typedef struct {
	URoom* ConnectedRoom;
	DoorStatus Status;
} Door;

UCLASS()
class THESHELTERS_API URoom : public UObject
{
	GENERATED_BODY()
private:
	int roomNo;
	UMonster* monster;
	RoomStatus roomStatus;

	Location location;

	// NOTE: Can be changed to room type
	ResourceType roomType;
	// To save resources
	int storage;

	std::map<Direction, Door> doors;

	bool cctv;

	bool panicRoomConnection;

public:
	URoom();
	URoom(const int no);
	int GetRoomNo();
	void SetLocation(const unsigned int x, const unsigned int y);
	Location GetLocation();
	bool IsConnectedToPanicRoom();

	const Door* GetDoor(const Direction d);
	void SetDoor(const Direction d, URoom* room, DoorStatus s);
	void OpenDoor(const Direction d);
	void CloseDoor(const Direction d);

	const UMonster* GetMonster();
	void InsertMonster(UMonster* newMonster);
	UMonster* DeleteMonster();
};
