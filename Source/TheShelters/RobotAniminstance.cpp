// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotAniminstance.h"

URobotAniminstance::URobotAniminstance()
{
	isMoving = false;
}

void URobotAniminstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		//isMoving = Pawn->GetVelocity().Size();
	}
}

void URobotAniminstance::SetMovement(bool _isMoving)
{
	isMoving = _isMoving;
}

