// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <vector>

#include "Room.h"
#include "MonsterActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoomControl.generated.h"

// key = monsterId, value = roomNum of the room monster is in
typedef TMap<int32, int32> MonsterLocationList;
// key = monsterId, value = UMonster class instance
typedef TMap<int32, UMonster *> MonsterList;

/* << ARoomControl : AActor >>
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
class THESHELTERS_API ARoomControl : public AActor
{
    GENERATED_BODY()

  public:
    // Constructors and Initializers
    ARoomControl();
    void InitGame(const unsigned int m, const unsigned int n);

    // Blueprint Callable Functions
    UFUNCTION(BlueprintCallable)
    void TestScenario();
    UFUNCTION(BlueprintCallable)
    void EndTurn();
    UFUNCTION(BlueprintCallable)
    void InitCCTV(TArray<AActor *> _ZapPlanes, TArray<AActor *> _RoomActors);
    UFUNCTION(BlueprintCallable)
	void SelectCCTV();
    UFUNCTION(BlueprintCallable)
    void RestoreZap(AActor *_ZapPlane);

	void ZapCCTV(AActor *_CurrentZapPlane);

    // Functions to find something in GameMap
    URoom *FindRoomByLocation(const unsigned int x, const unsigned int y);
    URoom *FindRoomById(const int roomId);
    UMonster *FindMonsterById(const unsigned int id);

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
    TArray<AActor *> ZapPlanes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AMonsterActor>> MonsterSpawn;


    // To show in blueprint
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
    TArray<URoom *> GameMap;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  private:
    // Monster related values
    MonsterList monsters;
    MonsterLocationList monsterLocations;
    int nextMonsterId = 1;
	TArray<AMonsterActor*> monsterActors;

	bool myContains(int input_num);

    // GameMap size
    unsigned int maxWidth;
    unsigned int maxHeight;
    float startX = 4000.0f;
    float startY = 0.0f;
    float startZ = 190.0f;
    float interval = 700.0f;

    // For test and debugging
    void PrintMap();
    void PrintTestMessage(const TCHAR *testName, const int num, const bool success);
};
