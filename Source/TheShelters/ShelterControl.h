// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <stdlib.h>

#include "Shelter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShelterControl.generated.h"

enum AffectStatus {Appear, Disappear};

// key = monsterId, value = shelterNum of the room monster is in
typedef TMap<int32, int32> ShelterList;
// key = monsterId, value = UMonster class instance
typedef TMap<int32, UMonster*> MonsterList;

UCLASS()
class THESHELTERS_API AShelterControl : public AActor
{
	GENERATED_BODY()

private:
	
	MonsterList Monsters;
	ShelterList Shelters;

	unsigned int MaxHeight;
	unsigned int MaxWidth;

	int nextMonsterId = 1;

public:	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
	TArray<UShelter*> GameMap;

	// Sets default values for this actor's properties
	AShelterControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void EndTurn();
	UFUNCTION(BlueprintCallable)
	void InitLevel();

	void InitGame(const unsigned int m, const unsigned int n);

	UShelter* FindShelterByLocation(const unsigned int x, const unsigned int y);
	void InsertMonster(MonsterType monsterType, int shelterNum);
	//void InsertMonster(const unsigned int x, const unsigned int y);

	/*
	// set room's monsterId to 0 and free monster instance
	void DeleteMonster(UShelter shelter);
	void DeleteMonster(const unsigned int x, const unsigned int y);
	*/
	// move monster to certain direction d according to monsterId
	bool MoveMonster(int monsterId, Direction d);
	void AffectShelters(int monsterId, int status);
	UMonster* FindMonsterById(const unsigned int id);
};
