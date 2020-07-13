// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

// constructor sets monster characteristics and its id. depends on MonsterType t
void UMonster::InitMonster(MonsterType t, int id)
{
    monsterId = id;

    if (t == DefaultMonster) {
        radioactive = false;
        emp = false;
        armorpierce = false;
        speed = 1;
    }
}

UMonster::UMonster() {}
// returns monsterId. Id for individual instances of monsters
const int             UMonster::MonsterId() const { return monsterId; }
// returns monsterType. type of monsters can be same among individuals
const MonsterType     UMonster::Type()      const { return monsterType; }
const int             UMonster::Speed()     const { return speed; }
const MonsterProperty UMonster::Property()  const { return MakeTuple(radioactive, emp, armorpierce); }