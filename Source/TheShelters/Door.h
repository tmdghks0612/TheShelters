#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Door.generated.h"

class URoom;

UENUM()
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

    const unsigned int Id();
    const DoorStatus Status() const;

  private:
    UPROPERTY()
    TArray<URoom *> connectedRooms;

    DoorStatus status;
    unsigned int id;
    static unsigned int nextId;
};