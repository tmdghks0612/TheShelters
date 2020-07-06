// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheSheltersGameMode.h"
#include "TheSheltersHUD.h"
#include "TheSheltersCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheSheltersGameMode::ATheSheltersGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATheSheltersHUD::StaticClass();
}
