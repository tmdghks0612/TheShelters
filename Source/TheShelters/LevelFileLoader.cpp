// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelFileLoader.h"

bool ULevelFileLoader::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::GetProjectFilePath() + FileNameB));
}

bool ULevelFileLoader::FileLoadString(FString FileNameA, FString& SaveTextA, int LevelNum)
{
	FString LevelFileExtension = ".txt";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(FPaths::ProjectContentDir() + FileNameA + FString::FromInt(LevelNum) + LevelFileExtension));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *(FPaths::GetProjectFilePath() + FileNameA + FString::FromInt(LevelNum) + LevelFileExtension));
	
	bool bExists = IFileManager::Get().FileExists(*(FPaths::ProjectContentDir() + FileNameA + FString::FromInt(LevelNum) + LevelFileExtension));
	if (bExists) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FileNameA);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("File not found"));
	}
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectContentDir() + FileNameA + FString::FromInt(LevelNum) + LevelFileExtension));
}

