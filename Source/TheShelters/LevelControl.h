// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>

#include "Monster.fwd.h"
#include "LevelControl.fwd.h"

#include "Direction.h"
#include "DoorActor.h"
#include "Room.h"
#include "RoomActor.h"
#include "SurvivorStat.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelControl.generated.h"

class AMonster;

// key = monsterId, value = roomNum of the room monster is in
typedef TMap<int32, int32> MonsterLocationList;
// key = monsterId, value = AMonster class instance
typedef TMap<int32, AMonster *> MonsterList;


USTRUCT(BlueprintType) 
struct FResourceUI {
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

  UFUNCTION(BlueprintCallable)
  TArray<FResourceUI> GetRoomResourceUI();
  UFUNCTION(BlueprintCallable)
  TArray<int> GetDoorUI();

    void ZapCCTV(AActor *_CurrentZapPlane);

    // Check if monster can enter panic room
    bool IsBlocked(int _monsterId);
    //Check if Robot tries to access blocked room
    bool IsRoomClosed(int roomNum, int direction); //For RobotControl Usage. 1 = up, 2 = right, 3 = down, 4 = left

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
    TArray<TSubclassOf<ADoorActor>> DoorActor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARoomActor> RoomActor;

    // To show in blueprint
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<URoom *> GameMap;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  private:
    // Initializers
    void InitRooms();
    void InitMap(FString LevelString);
    void InitPanicRoom(); // Must call after InitRooms
    void InitSurvivorStat();

    // spawn room mesh for visible rooms
    void SpawnRoomMesh(int roomNum);
    void SpawnDoorMesh(int roomNum);

    // Monster related values
    MonsterList monsters;
    MonsterLocationList monsterLocations;
    int nextMonsterId = 1;
    TArray<AMonster *> monsterActors;
    Direction ChooseWeightedRandomDirection(TMap<Direction, int32> weights);

  // Resource Room numbers
  int maxFoodRoom = 3;
  int maxWaterRoom = 3;
  int maxElectricityRoom = 3;

  TArray<int> foodRoomNum;
  TArray<int> waterRoomNum;
  TArray<int> electricityRoomNum;

  // Special room minimum distance
  int resourceRoomDistance = 5;

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
    TMap<FString, bool> eventFlag;

    // Player related values
    USurvivorStat *survivorStat;

  // For test and debugging
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
  void SetRoomResources(int RoomId, int food, int water, int electricity);
  UFUNCTION()
  void RobotCheck(int RoomId);

  UFUNCTION(BlueprintCallable)
  int GetMaxWidth();
  UFUNCTION(BlueprintCallable)
  int GetMaxHeight();

};