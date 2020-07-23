// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RobotAniminstance.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API URobotAniminstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URobotAniminstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void SetMovement(bool _isMoving);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool isMoving;

private:
	
};
