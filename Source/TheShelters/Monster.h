// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Monster.generated.h"

// MonsterType determines MonsterProperty values
enum MonsterType { DefaultMonster };

// MonsterProperty <radioactive, emp, armorpierce>
typedef TTuple<bool, bool, bool> MonsterProperty;

/* << UMonster >>
 * Constructor:
 *     Default Constructor
 * Initializer:
 *     Argument: MonsterType, MonsterId
 */
UCLASS()
class THESHELTERS_API UMonster : public UObject
{
	GENERATED_BODY()

public:
	// Constructors and Initializers
	UMonster();
	void InitMonster(MonsterType t, int id);
	int GetMonsterId();
	MonsterType GetType();

	bool IsRadioactive();

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
