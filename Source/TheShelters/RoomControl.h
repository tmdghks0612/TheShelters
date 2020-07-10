// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Room.h"
#include <vector>
#include <utility>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomControl.generated.h"

typedef std::vector<std::pair<UMonster*, URoom*>> MonsterList;

UCLASS()
class THESHELTERS_API ARoomControl : public AActor
{
	GENERATED_BODY()
private:

	URoom** GameMap;
	MonsterList Monsters;

	unsigned int MaxHeight;
	unsigned int MaxWidth;
public:	

	// Sets default values for this actor's properties
	ARoomControl();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	void InitGame(const unsigned int m, const unsigned int n);

	URoom* FindRoomByLocation(const unsigned int x, const unsigned int y);
	void InsertMonster(UMonster* monster, URoom* room);
	void InsertMonster(UMonster* monster, const unsigned int x, const unsigned int y);

	void DeleteMonster(URoom* room);
	void DeleteMonster(const unsigned int x, const unsigned int y);

	void MoveMonster(UMonster* monster, Direction d);
	void MoveMonster(int monsterId, Direction d);

	UMonster* FindMonsterById(const unsigned int id);
};
