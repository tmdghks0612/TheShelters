#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "Door.generated.h"

class URoom;

UENUM(BlueprintType)
enum class DoorStatus : uint8
{
    Open,
    Close
};

UCLASS()
class THESHELTERS_API ADoor : public AStaticMeshActor
{
    GENERATED_BODY()

  public:
    ADoor();
    ~ADoor();

    UFUNCTION()
    void InitDoor(URoom *roomA, URoom *roomB, TEnumAsByte<DoorStatus> s);

    UFUNCTION()
    void Open();

    UFUNCTION()
    void Close();

    UFUNCTION()
    URoom *GetBeyond(URoom *room);

    UFUNCTION()
    void SwitchStatus();

    const unsigned int Id();
    const TEnumAsByte<DoorStatus> Status() const;

  private:
    UPROPERTY()
    TArray<URoom *> connectedRooms;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<DoorStatus> status;
    unsigned int id;
    static unsigned int nextId;
};