// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Room.h"
#include "GameFramework/SaveGame.h"
#include "ShelterGameSave.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API UShelterGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UShelterGameSave();
	int foodNeed;
	int waterNeed;
	float electNeed;
	int day;
	double mental;

	UFUNCTION()
	int32 GetCCTVData(int i);
	UFUNCTION()
	int GetFood(int i);
	UFUNCTION()
	int GetWater(int i);
	UFUNCTION()
	float GetElect(int i);
	UFUNCTION()
	bool GetLeft(int i);
	UFUNCTION()
	bool GetRight(int i);
	UFUNCTION()
	bool GetUp(int i);
	UFUNCTION()
	bool GetDown(int i);
	UFUNCTION()
	bool GetisKnown(int i);

	UFUNCTION()
	void SetRoomData(int index, int _food, int _water, float _elect, bool _Left, bool _Right, bool _Up, bool _Down, bool isKnown);
	UFUNCTION()
	void SetCCTVData(int index, int32 Number);

private:

	UPROPERTY()
	TArray<int32> CCTVData;
	UPROPERTY()
	TArray<int> food;
	UPROPERTY()
	TArray<int> water;
	UPROPERTY()
	TArray<float> electricity;
	UPROPERTY()
	TArray<bool> LeftDoor;
	UPROPERTY()
	TArray<bool> RightDoor;
	UPROPERTY()
	TArray<bool> UpDoor;
	UPROPERTY()
	TArray<bool> DownDoor;
	UPROPERTY()
	TArray<bool> isKnown;
	

	bool isGenerated;
	
};