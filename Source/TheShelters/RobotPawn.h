// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotAniminstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RobotPawn.generated.h"

UCLASS()
class THESHELTERS_API ARobotPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARobotPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	bool isMoving;

	UFUNCTION()
	void SetMovement(bool _Move);

	UFUNCTION()
	bool GetMovement();

	UFUNCTION()
	void SetArrival(bool _isArrived);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotActor")
	USkeletalMesh* RobotSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* RobotSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URobotAniminstance* RobotAnimInstance;

};
