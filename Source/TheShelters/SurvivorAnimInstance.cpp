// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorAnimInstance.h"

USurvivorAnimInstance::USurvivorAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
}

void USurvivorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}
