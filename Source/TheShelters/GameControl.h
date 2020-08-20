// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShelterGameSave.h"
#include "Kismet/GameplayStatics.h"
#include "Room.h"
#include "Engine/GameInstance.h"
#include "GameControl.generated.h"

//class UShelterGameSave;
/**
 * 
 */
UCLASS()
class THESHELTERS_API UGameControl : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameControl();

	UFUNCTION()
	TArray<URoom*> GetRoomData();
	
	UFUNCTION()
	bool CheckGenerated();
	UFUNCTION()
	void SetIsGenerated(bool _isGenerated);
	UFUNCTION()
	void SetGameMapData(int i, URoom* _GameMap);
	UFUNCTION()
	URoom* GetGameMapData(int i);
	UFUNCTION()
	bool CheckCCTV();
	UFUNCTION()
	void SetCCTVData(int i, int32 roomNum);
	UFUNCTION()
	int32 GetCCTVData(int i);
	UFUNCTION()
	void LoadSaveData();
	UFUNCTION()
	bool CheckLoaded();
	UFUNCTION()
	void SetisLoaded(bool _Loaded);
	UFUNCTION()
	void SetDay(int _day);
	UFUNCTION()
	void SetProgress(int _progress);

	UFUNCTION()
	int GetDay();
	UFUNCTION()
	int GetProgress();
	

private:
	UPROPERTY()
	TArray<URoom*> RoomData;
	UPROPERTY()
	TArray<int32> CCTVData;
	int foodNeed;
	int waterNeed;
	float electNeed;
	int day;
	double mental;
		
	bool isLoaded;
	bool isGenerated;
	int progress;
};
