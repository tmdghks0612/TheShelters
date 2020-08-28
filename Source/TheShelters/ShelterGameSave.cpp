// Fill out your copyright notice in the Description page of Project Settings.


#include "ShelterGameSave.h"

UShelterGameSave::UShelterGameSave()
{
	CCTVData.Empty();
	CCTVData.SetNum(12);
	CCTVData[0] = -1;
}

int32 UShelterGameSave::GetCCTVData(int i)
{
	return CCTVData[i];
}

int UShelterGameSave::GetFood(int i)
{
	return food[i];
}

int UShelterGameSave::GetWater(int i)
{
	return water[i];
}

float UShelterGameSave::GetElect(int i)
{
	return electricity[i];
}

bool UShelterGameSave::GetLeft(int i)
{
	return LeftDoor[i];
}

bool UShelterGameSave::GetRight(int i)
{
	return RightDoor[i];
}

bool UShelterGameSave::GetUp(int i)
{
	return UpDoor[i];
}

bool UShelterGameSave::GetDown(int i)
{
	return DownDoor[i];
}

bool UShelterGameSave::GetisKnown(int i)
{
	return isKnown[i];
}

void UShelterGameSave::SetRoomData(int index, int _food, int _water, float _elect, bool _Left, bool _Right, bool _Up, bool _Down, bool _isKnown)
{
	if (food.Num() <= index)
	{
		food.Add(_food);
		water.Add(_water);
		electricity.Add(_elect);
		LeftDoor.Add(_Left);
		RightDoor.Add(_Right);
		UpDoor.Add(_Up);
		DownDoor.Add(_Down);
		isKnown.Add(_isKnown);

	}
	else
	{
		food[index] = _food;
		water[index] = _water;
		electricity[index] = _elect;
		LeftDoor[index] = _Left;
		RightDoor[index] = _Right;
		UpDoor[index] = _Up;
		DownDoor[index] = _Down;
		isKnown[index] = _isKnown;
	}
}

void UShelterGameSave::SetCCTVData(int index, int32 Number)
{
	if (CCTVData.Num() <= index)
	{
		CCTVData.Add(Number);
	}
	else
	{
		CCTVData[index] = Number;
	}
}

void UShelterGameSave::SetDayProgress(int _day, int _progress)
{
	day = _day;
	progress = _progress;
	UE_LOG(LogTemp, Warning, TEXT("Day is %d Progress is %d in save data"), day, progress);
}

int UShelterGameSave::GetDay()
{
	return day;
}

int UShelterGameSave::GetProgress()
{
	return progress;
}
