// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameControl.h"
#include "CoreMinimal.h"
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

private:
	TArray<UGameControl> SaveData;
};
