// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Direction.h"
#include "Door.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Room.generated.h"
class URoom;

UENUM() enum class RoomType { None, Food, Water, Electricity };

UENUM() enum class RoomStatus { Peaceful, Dangerous };

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

    // Getters and Setters
    const int RoomId() const;
    const int MonsterId() const;
    ADoor *GetDoor(const Direction d);
    URoom *BeyondDoor(const Direction d);

    void SetDoor(const Direction d, ADoor *door);
    void OpenDoor(const Direction d);
    void CloseDoor(const Direction d);
    void InsertMonster(int newMonsterId);
    void DeleteMonster();

	Resource GetResources();
    void SetResources(unsigned int _food, unsigned int _water, unsigned int _electricity);
	void InitResources(RoomType _roomType);
	bool isDiscovered();

    void SetisKnown(bool _check);

    void Radiated();

  private:
    // Default Room values
    int roomId;

    // Keep changing variables
    bool cctv;
    int monsterId; // 0 means no monster
    RoomStatus roomStatus;
    bool isKnown;
    UPROPERTY()
    TArray<ADoor *> doors;

    // Resource properties
    int resourceThreshold = 5;
    Resource resources;
};