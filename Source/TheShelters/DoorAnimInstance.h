// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DoorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API UDoorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDoorAnimInstance();

	UFUNCTION(BlueprintCallable)
	void SetDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool isClosed;
};
