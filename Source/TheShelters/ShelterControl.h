// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <stdlib.h>

#include "Shelter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShelterControl.generated.h"

// key = monsterId, value = shelterNum of the room monster is in
typedef TMap<int32, int32>     MonsterLocationList;
// key = monsterId, value = UMonster class instance
typedef TMap<int32, UMonster*> MonsterList;

/* << AShelterControl : AActor >>
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
class THESHELTERS_API AShelterControl : public AActor
{
	GENERATED_BODY()

public:	
	// Constructors and Initializers
	AShelterControl();
	void InitGame(const unsigned int m, const unsigned int n);

	// Blueprint Callable Functions
	UFUNCTION(BlueprintCallable)
	void TestScenario();
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	// Functions to find something in GameMap
	UShelter* FindShelterByLocation(const unsigned int x, const unsigned int y);
	UShelter* FindShelterById(const int shelterId);
	UMonster* FindMonsterById(const unsigned int id);

	// Monster's behavior
	// Monsters will be created when inserted
	void InsertMonster(MonsterType monsterType, int x, int y);
	void InsertMonster(MonsterType monsterType, int shelterId);
	// To delete monster from Shelter, set Shelter's monsterId to 0.
	void DeleteMonster(const unsigned int x, const unsigned int y);
	void DeleteMonster(int shelterId);
	// Move the given mosnter's location to the direction d.
	bool MoveMonster(int monsterId, Direction d);

	// To show in blueprint
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
	TArray<UShelter*>	GameMap;

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