// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "Direction.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Room.generated.h"
class URoom;

enum RoomType {
	None,
	Food,
	Water,
	Electricity
};

enum RoomStatus
{
    Peaceful,
    Dangerous
};
enum DoorStatus
{
    Open,
    Close
};

typedef struct
{
	unsigned int food;
	unsigned int water;
	unsigned int electricity;
} Resource;

typedef struct
{
    unsigned int x;
    unsigned int y;
} Location;

typedef struct
{
    URoom *connectedRoom;
    DoorStatus status;
} Door;

/* << URoom : UObject >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitRoom: RoomId
 * Property:
 * - ID
 * - MonsterId
 * - RoomType
 * - Doors
 * - RoomStatus
 * - CCTV
 *
 * Description:
 * A Room is a object which contains a monster and resource. It is
 * connected to the other Rooms by doors.
 */

UCLASS()
class THESHELTERS_API URoom : public UObject
{
    GENERATED_BODY()

  public:
    // Constructors and Initializers
    URoom();
    ~URoom();
    void InitRoom(const int num);
    void InitDoor(const Direction d, URoom *connectedRoom, DoorStatus s);

    // Getters and Setters
    const int RoomId() const;
    const int MonsterId() const;
    const Door GetDoor(const Direction d);

    void SetDoor(const Direction d, const DoorStatus s);
    void OpenDoor(const Direction d);
    void CloseDoor(const Direction d);
    void InsertMonster(int newMonsterId);
    void DeleteMonster();

	Resource GetResources();
	void InitResources(RoomType _roomType);
	bool isDiscovered();

    void Radiated();

  private:
    // Default Room values
    int roomId;

    // Keep changing variables
    bool cctv;
    int monsterId; // 0 means no monster
    RoomStatus roomStatus;
	bool isKnown;
    std::map<Direction, Door> doors;

    // Resource properties
	int resourceThreshold = 5;
	Resource resources;
};