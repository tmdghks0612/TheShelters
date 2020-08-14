// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Room.h"
#include "Engine/GameInstance.h"
#include "GameControl.generated.h"

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

private:
	UPROPERTY()
	TArray<URoom*> RoomData;
	bool isGenerated;
};
