// Fill out your copyright notice in the Description page of Project Settings.

#include "GameControl.h"

UGameControl::UGameControl()
{
	RoomData.Empty();
	CCTVData.Empty();
	isGenerated = false;
	isLoaded = false;
	RoomData.SetNum(100);
	CCTVData.SetNum(12);
	CCTVData[0] = -1;

	foodNeed = 1;
	waterNeed = 1;
	electNeed = 1;
	day = 0;
	progress = 0;
	MaxProgress = 10;
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
}

void UGameControl::LoadSaveData()
{
	if (UShelterGameSave* LoadedGame = Cast<UShelterGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("SAVE"), 0)))
	{
		for (int i = 0; i < 100; i++)
		{
			RoomData[i]->SetFood(LoadedGame->GetFood(i));
			RoomData[i]->SetWater(LoadedGame->GetWater(i));
			RoomData[i]->SetElectricity(LoadedGame->GetElect(i));
			
			if (LoadedGame->GetLeft(i) == true)
			{
				RoomData[i]->OpenDoor(Direction::Left);
			}
			else
			{
				RoomData[i]->CloseDoor(Direction::Left);
			}

			if (LoadedGame->GetRight(i) == true)
			{
				RoomData[i]->OpenDoor(Direction::Right);
			}
			else
			{
				RoomData[i]->CloseDoor(Direction::Right);
			}

			if (LoadedGame->GetUp(i) == true)
			{
				RoomData[i]->OpenDoor(Direction::Up);
			}
			else
			{
				RoomData[i]->CloseDoor(Direction::Up);
			}

			if (LoadedGame->GetDown(i) == true)
			{
				RoomData[i]->OpenDoor(Direction::Down);
			}
			else
			{
				RoomData[i]->CloseDoor(Direction::Down);
			}

			RoomData[i]->SetisKnown(LoadedGame->GetisKnown(i));
		}
		for (int i = 0; i < 12; i++)
		{
			CCTVData[i] = LoadedGame->GetCCTVData(i);
		}
		UE_LOG(LogTemp, Warning, TEXT("LOADED"));
		SetIsGenerated(true);
	}
	
}

bool UGameControl::CheckLoaded()
{
	return isLoaded;
}

void UGameControl::SetisLoaded(bool _Loaded)
{
	isLoaded = _Loaded;
}

void UGameControl::SetDay(int _day)
{
	day = _day;
}

void UGameControl::SetProgress(int _progress)
{
	progress = _progress;
}

int UGameControl::GetDay()
{
	return day;
}

int UGameControl::GetProgress()
{
	return progress;
}

int UGameControl::GetMaxProgress()
{
	return MaxProgress;
}



int32 UGameControl::GetCCTVData(int i)
{
	return CCTVData[i];
}
