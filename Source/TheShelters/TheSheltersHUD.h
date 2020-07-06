// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TheSheltersHUD.generated.h"

UCLASS()
class ATheSheltersHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATheSheltersHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

