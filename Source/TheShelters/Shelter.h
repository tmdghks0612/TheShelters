// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "Monster.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Shelter.generated.h"

/**
 * 
 */

class UShelter;

enum ResourceType { Empty, Electricity, Water, Food, Metal, Circuit };
enum ShelterStatus { Peaceful, Dangerous };
enum DoorStatus { Open, Close };
enum Direction { Left, Right, Up, Down };

typedef struct {
	unsigned int x;
	unsigned int y;
} Location;

typedef struct {
	int connectedShelter;
	DoorStatus status;
} Door;

UCLASS()
class THESHELTERS_API UShelter : public UObject
{
	GENERATED_BODY()
	
private:
	int shelterNum;
	// monsterid 0 means no monster.
	int monsterId;
	ShelterStatus shelterStatus;

	Location location;

	// NOTE: Can be changed to room type
	ResourceType shelterType;
	// To save resources
	int storage;

	std::map<Direction, Door> doors;

	bool cctv;
	bool panicRoomConnection;

public:
	UShelter();
	void InitShelter(const int num);
	int GetShelterNum();
	void SetLocation(const unsigned int x, const unsigned int y);
	Location GetLocation();
	bool IsConnectedToPanicRoom();

	Door GetDoor(const Direction d);
	void SetDoor(const Direction d, int _shelterNum, DoorStatus s);
	void OpenDoor(const Direction d);
	void CloseDoor(const Direction d);

	int GetMonsterId();
	void InsertMonster(int newMonsterId);
	void DeleteMonster();

	void Radiated();
};
