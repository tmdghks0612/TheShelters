// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "Monster.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Shelter.generated.h"


class UShelter;

enum ResourceType  { Empty, Electricity, Water, Food, Metal, Circuit };
enum ShelterStatus { Peaceful, Dangerous };
enum DoorStatus    { Open, Close };
enum Direction     { Left, Right, Up, Down };

typedef struct {
	unsigned int x;
	unsigned int y;
} Location;

typedef struct {
	int connectedShelter;
	DoorStatus status;
} Door;

/* << UShelter : UObject >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitShelter: ShelterId
 * Property:
 * - ID
 * - MonsterId
 * - ShelterType
 * - Doors
 * - ShelterStatus
 * - CCTV
 * - PanicRoomConnection
 * 
 * Description:
 * A Shelter is a object which contains a monster and resource. It is
 * connected to the other Shelters by doors.
 */

UCLASS()
class THESHELTERS_API UShelter : public UObject
{
	GENERATED_BODY()

public:
	// Constructors and Initializers
	UShelter();
	void InitShelter(const int num);
	void InitDoor(const Direction d, int _shelterId, DoorStatus s);

	// Getters and Setters
	const int  ShelterId()                const;
	const int  MonsterId()				  const;
	const bool IsConnectedToPanicRoom()   const;
	const Door GetDoor(const Direction d);

	void SetDoor(const Direction d, const DoorStatus s);
	void InsertMonster(int newMonsterId);
	void DeleteMonster();

	void Radiated();

private:
	// Default Shelter values
	int shelterId;

	// Keep changing variables
	bool cctv;
	int monsterId;  // 0 means no monster
	ShelterStatus shelterStatus;
	std::map<Direction, Door> doors;

	// Shelter Properties
	ResourceType shelterType;
	bool panicRoomConnection;
};
