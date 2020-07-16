// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Monster.generated.h"

// MonsterType determines MonsterProperty values and movement speed
enum MonsterType
{
    DefaultMonster
};

// MonsterProperty <radioactive, emp, armorpierce>
typedef TTuple<bool, bool, bool> MonsterProperty;

/* << UMonster : UObject >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitMonster: MonsterType, MonsterId
 * Property:
 * - ID
 * - Type
 * - Speed
 * - Radioactive
 * - Emp
 * - Armorpierce
 *
 * Description:
 * A monster is just a data class. Every behavior is delegated to ShelterControl.
 * Monsters have ID, which is unique. It has type, which determines monster's
 * all property values. Monsters have speed, which determines maximum distance
 * to move for each turn. And monsters have three properties. Radioactive spoil
 * nearby food and water. Emp break nearby CCTVs and doors up. Armorpiercing
 * monster can break armor up and give fatal damage to the player.
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
    const int MonsterId() const;
    const MonsterType Type() const;
    const int Speed() const;
    const MonsterProperty Property() const;

  protected:
  private:
    UPROPERTY(EditAnywhere)
    // Default Monster values
    int monsterId;
    MonsterType monsterType;

    // Monster Properties
    bool radioactive;
    bool emp;
    bool armorpierce;

    // Monster movement speed
    int speed;
};
