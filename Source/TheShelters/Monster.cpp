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

// returns monsterId. Id for individual instances of monsters
int UMonster::GetMonsterId()
{
	return monsterId;
}

// returns monsterType. type of monsters can be same among individuals
MonsterType UMonster::GetType()
{
	return monsterType;
}

bool UMonster::IsRadioactive()
{
	return radioactive;
}

// default functions


UMonster::UMonster() {
	
}
