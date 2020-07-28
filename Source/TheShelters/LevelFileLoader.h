// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelFileLoader.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API ULevelFileLoader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "save")
	static bool FileSaveString(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintPure, Category = "save")
	static bool FileLoadString(FString FileNameA, FString& SaveTextA, int LevelNum);
};
