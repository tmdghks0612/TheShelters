// Fill out your copyright notice in the Description page of Project Settings.

#include "GameControl.h"

UGameControl::UGameControl()
{
	RoomData.Empty();
	CCTVData.Empty();
	isGenerated = false;
	RoomData.SetNum(100);
	CCTVData.SetNum(12);
	CCTVData[0] = -1;
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

bool UGameControl::CheckCCTV()
{
	if (CCTVData[0] == -1)
		return false;
	else 
		return true;
}

void UGameControl::SetCCTVData(int i, int32 roomNum)
{
	CCTVData[i] = roomNum;
	UE_LOG(LogTemp, Warning, TEXT("CCTV is at %d"), roomNum);
	
}

int32 UGameControl::GetCCTVData(int i)
{
	return CCTVData[i];
}
