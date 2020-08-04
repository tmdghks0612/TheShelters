// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THESHELTERS_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void SetAngry(bool _isAngry);

	UFUNCTION(BlueprintCallable)
	void SetMovement(bool _isMoving);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool isAngry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool isMoving;
};
