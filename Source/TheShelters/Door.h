#pragma once

#include "Door.fwd.h"
#include "Room.fwd.h"

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "Door.generated.h"

enum DoorStatus
{
    DoorStatusOpen,
    DoorStatusClose
};

UCLASS()
class THESHELTERS_API ADoor : public AStaticMeshActor
{
    GENERATED_BODY()

  public:
    ADoor();
    ~ADoor();

    void InitDoor(URoom *roomA, URoom *roomB, DoorStatus s);

    void Open();
    void Close();

    URoom *GetBeyond(URoom *room);

    const unsigned int Id() const;
    const DoorStatus Status() const;

  private:
    TArray<URoom *> connectedRooms;
    DoorStatus status;
    unsigned int id;
    unsigned int nextId = 1;
};