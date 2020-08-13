// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "DoorAnimInstance.h"
#include "PanicRoomDoor.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API APanicRoomDoor : public ADoor
{
	GENERATED_BODY()
	
public:
	APanicRoomDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDoorAnimInstance* Animinstance;

	UFUNCTION()
	void SetDoor();



protected:


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorActor")
	USkeletalMesh* DoorSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* DoorSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimBlueprint* AnimationBP;
};
