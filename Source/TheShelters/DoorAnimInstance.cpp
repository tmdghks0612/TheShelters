// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorAnimInstance.h"

UDoorAnimInstance::UDoorAnimInstance()
{
	isClosed = false;
}


void UDoorAnimInstance::SetDoor()
{
	isClosed = !isClosed;
}
