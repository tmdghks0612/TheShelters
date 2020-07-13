// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Monster.generated.h"

// MonsterType determines MonsterProperty values and movement speed
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

    // Getters and Setters
    const int             MonsterId() const;
    const MonsterType     Type()      const;

    const int             Speed()     const;

    // Monster behaviors
    const MonsterProperty Affect()    const;

protected:

private:
    UPROPERTY(EditAnywhere)
    // Default Monster values
    int         monsterId;
    MonsterType monsterType;

    // Monster Properties
    bool        radioactive;
    bool        emp;
    bool        armorpierce;

    // Monster movement speed
    int         speed;
};
