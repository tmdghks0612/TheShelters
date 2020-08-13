// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>

#include "Direction.h"
#include "Door.h"
#include "PanicRoomDoor.h"
#include "DoorAnimInstance.h"
#include "Monster.h"
#include "Room.h"
#include "RoomActor.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LevelControl.generated.h"

class AMonster;


// key = monsterId, value = roomNum of the room monster is in
typedef TMap<int32, int32> MonsterLocationList;
// key = monsterId, value = AMonster class instance
typedef TMap<int32, AMonster *> MonsterList;

USTRUCT(BlueprintType)
struct FResourceUI
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 resourceType;
    UPROPERTY(BlueprintReadOnly)
    int32 resourceSize;
};

/* << ALevelControl : AActor >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitGame: MaxHeight, MaxWidth
 *
 * Property:
 *
 *
 * Description:
 *
 *
 */
UCLASS()
class THESHELTERS_API ALevelControl : public AActor
{
    GENERATED_BODY()

  public:
    // Constructors and Initializers
    ALevelControl();
    void InitGame(const unsigned int m, const unsigned int n, FString _LevelString);

  // Blueprint Callable Functions
  UFUNCTION(BlueprintCallable)
  void TestScenario(FString _LevelString); // For test
  UFUNCTION(BlueprintCallable)
  void EndTurn();
  UFUNCTION(BlueprintCallable)
  void InitCCTV(TArray<AActor *> _ZapPlanes, TArray<AActor *> _RoomActors);
  UFUNCTION(BlueprintCallable)
  void SelectCCTV();
  UFUNCTION(BlueprintCallable)
  void RestoreZap(AActor *_ZapPlane);
  UFUNCTION(BlueprintCallable)
  void InitDoorMesh();
  UFUNCTION(BlueprintCallable)
  void InitVisibleRoom();
  UFUNCTION(BlueprintCallable)
  bool CheckPanicRoom(int _monsterId);
  // Use electricity if electricity is enough.
  UFUNCTION(BlueprintCallable)
  void UseElectricity();

  UFUNCTION(BlueprintCallable)
  float GetElectricityPercent();

  UFUNCTION(BlueprintCallable)
  void DoorSwitch(Direction d);

  UFUNCTION(BlueprintCallable)
  int GetPanicRoomFood();
  UFUNCTION(BlueprintCallable)
  int GetPanicRoomWater();

  UFUNCTION(BlueprintCallable)
  void SetPanicRoomFood(int _value);
  UFUNCTION(BlueprintCallable)
  void SetPanicRoomWater(int _value);

  UFUNCTION(BlueprintCallable)
  TArray<FResourceUI> GetRoomResourceUI();
  UFUNCTION(BlueprintCallable)
  TArray<DoorStatus> GetDoorUI();

    void ZapCCTV(AActor *_CurrentZapPlane);

    // Check if monster can enter panic room
    bool IsBlocked(int _monsterId);
    // Check if Robot tries to access blocked room
    bool IsRoomClosed(int roomNum, int direction); // For RobotControl Usage. 1 = up, 2 = right, 3 = down, 4 = left

    // Functions to find something in GameMap
    URoom *FindRoomByLocation(const unsigned int x, const unsigned int y);
    URoom *FindRoomById(const int roomId);
    AMonster *FindMonsterById(const unsigned int id);

    // Monster's behavior
    // Monsters will be created when inserted
    void InsertMonster(MonsterType monsterType, int x, int y);
    void InsertMonster(MonsterType monsterType, int roomId);
    // To delete monster from Room, set Room's monsterId to 0.
    void DeleteMonster(const unsigned int x, const unsigned int y);
    void DeleteMonster(int roomId);
    // Move the given mosnter's location to the direction d.
    bool MoveMonster(int monsterId, Direction d);

	// Check if electricity is enough to use on panic room. return true if enough, otherwise false
	bool IsElectricityEnough();

    // cctv room number array and its zap planes accordingly
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<int32> CCTVRoomNum;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<int32> VisibleRoomNum;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<AActor *> ZapPlanes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UBlueprint *> SpawnActor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<AMonster>> MonsterSpawn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<ADoor>> DoorActor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APanicRoomDoor> PanicRoomDoor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARoomActor> RoomActor;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray <APanicRoomDoor*> PanicRoomDoorList;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  private:

	// To show in blueprint
	UPROPERTY()
	TArray<URoom *> GameMap;

    // Initializers
    void InitRooms();
    void InitMap(FString LevelString);
    void InitPanicRoom(); // Must call after InitRooms

    // spawn room mesh for visible rooms
    void SpawnRoomMesh(int roomNum);
    void SpawnDoorMesh(int roomNum);

    // Monster related values
    MonsterList monsters;
    MonsterLocationList monsterLocations;
    int nextMonsterId = 1;
    UPROPERTY()
    TArray<AMonster *> monsterActors;
    Direction ChooseWeightedRandomDirection(TMap<Direction, int32> weights);

    // Resource Room numbers
    int maxFoodRoom = 3;
    int maxWaterRoom = 3;
    int maxElectricityRoom = 3;

  UPROPERTY()
  TArray<int> foodRoomNum;
  UPROPERTY()
  TArray<int> waterRoomNum;
  UPROPERTY()
  TArray<int> electricityRoomNum;

    // Special room minimum distance
    int resourceRoomDistance = 5;

  // Electricity on panic room related variables
  int electricityUsage = 1;
  float electricityDecreaseSpeed = 0.05f;
  

  // maximum resources capacity
  int maxElectricity = 100;
  int maxFood = 50;
  int maxWater = 100;

  // Panic Room related values
  int panicRoomId = 5;

    bool MyContains(int input_num);
    bool IsNextPanicRoom(int roomNumber);

    // GameMap size
    unsigned int maxWidth;
    unsigned int maxHeight;
    float startX = 4000.0f;
    float startY = 0.0f;
    float startZ = 200.0f;
    float interval = 1400.0f;

    // Event flag
    UPROPERTY()
    TMap<FString, bool> eventFlag;

  // For test and debugging
  UPROPERTY()
  TMap<bool, int32> testResult;
  void PrintMap();
  void PrintTestMessage(const TCHAR *testName, const int num, const bool success);

 public:
  // Get/Set functions
  UFUNCTION(BlueprintCallable)
  TArray<int32> GetCCTVRoomNum();

  UFUNCTION()
  int ResourceCheckByRobot(int RoomId, int Type);
  UFUNCTION()
  void SetRoomResources(int RoomId, int food, int water, float electricity);
  UFUNCTION()
  void RobotCheck(int RoomId);

  UFUNCTION(BlueprintCallable)
  int GetMaxWidth();
  UFUNCTION(BlueprintCallable)
  int GetMaxHeight();
};
