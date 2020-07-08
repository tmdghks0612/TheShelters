// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Monster.generated.h"

enum MonsterType { DefaultMonster };

/**
 * 
 */
UCLASS()
class THESHELTERS_API UMonster : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMonster();
	UMonster(MonsterType t, int id);
	int GetMonsterId();
	MonsterType GetType();

protected:

private:
	UPROPERTY(EditAnywhere)
	int monsterId;
	MonsterType monsterType;

	bool radioactive;
	bool emp;
	bool armorpierce;

	int speed;
};
