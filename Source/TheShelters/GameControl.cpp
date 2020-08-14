// Fill out your copyright notice in the Description page of Project Settings.

#include "GameControl.h"

UGameControl::UGameControl()
{
	RoomData.Empty();
	isGenerated = false;
	RoomData.SetNum(100);
}


TArray<URoom*> UGameControl::GetRoomData()
{
	return RoomData;
}

bool UGameControl::CheckGenerated()
{
	return isGenerated;
}

void UGameControl::SetIsGenerated(bool _isGenerated)
{
	isGenerated = _isGenerated;
}

void UGameControl::SetGameMapData(int i, URoom* _GameMap)
{
	RoomData[i] = _GameMap;
}

URoom* UGameControl::GetGameMapData(int i)
{
	return RoomData[i];
}