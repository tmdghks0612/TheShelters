// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include <map>

#include "CoreMinimal.h"
#include "Room.generated.h"
#include "UObject/NoExportTypes.h"

class URoom;

enum ResourceType
{
    Empty,
    Electricity,
    Water,
    Food,
    Metal,
    Circuit
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
enum Direction
{
    Left,
    Up,
    Right,
    Down
};

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
 * - PanicRoomConnection
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

    void Radiated();

  private:
    // Default Room values
    int roomId;

    // Keep changing variables
    bool cctv;
    int monsterId; // 0 means no monster
    RoomStatus roomStatus;
    std::map<Direction, Door> doors;

    // Room Properties
    ResourceType roomType;
};
