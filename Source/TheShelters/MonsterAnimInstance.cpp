// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	isAngry = false;
	isMoving = false;
}

void UMonsterAnimInstance::SetAngry(bool _isAngry)
{
	isAngry = _isAngry;
}

void UMonsterAnimInstance::SetMovement(bool _isMoving)
{
	isMoving = _isMoving;
}
