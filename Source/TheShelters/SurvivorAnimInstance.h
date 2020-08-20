// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SurvivorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API USurvivorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USurvivorAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
};
