// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <stdlib.h>

#include "Room.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomControl.generated.h"

// key = monsterId, value = roomNum of the room monster is in
typedef TMap<int32, int32>     MonsterLocationList;
// key = monsterId, value = UMonster class instance
typedef TMap<int32, UMonster*> MonsterList;

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

	// Functions to find something in GameMap
	URoom* FindRoomByLocation(const unsigned int x, const unsigned int y);
	URoom* FindRoomById(const int roomId);
	UMonster* FindMonsterById(const unsigned int id);

	// Monster's behavior
	// Monsters will be created when inserted
	void InsertMonster(MonsterType monsterType, int x, int y);
	void InsertMonster(MonsterType monsterType, int roomId);
	// To delete monster from Room, set Room's monsterId to 0.
	void DeleteMonster(const unsigned int x, const unsigned int y);
	void DeleteMonster(int roomId);
	// Move the given mosnter's location to the direction d.
	bool MoveMonster(int monsterId, Direction d);

	// To show in blueprint
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
	TArray<URoom*>	GameMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Monster related values
	MonsterList			monsters;
	MonsterLocationList monsterLocations;
			 int		nextMonsterId = 1;

	// GameMap size
	unsigned int		maxWidth;
	unsigned int		maxHeight;

	// For test and debugging
	void PrintMap();
	void PrintTestMessage(const TCHAR* testName, const int num, const bool success);
};
