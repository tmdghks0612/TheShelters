// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheSheltersGameMode.h"
#include "TheSheltersHUD.h"
#include "TheSheltersCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheSheltersGameMode::ATheSheltersGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/SurvivorInst/MySurvivor"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/SurvivorInst/MouseController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
	
	HUDClass = ATheSheltersHUD::StaticClass();
	
}