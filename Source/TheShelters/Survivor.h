// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class THESHELTERS_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivor();

	UFUNCTION(BlueprintCallable)
	void InitiateMap();

	UFUNCTION(BlueprintCallable)
	void InitRobots(ARobotControl *_Robot);

	UFUNCTION()

    void RobotMapRight();
    UFUNCTION()
    void RobotMapLeft();
    UFUNCTION()
    void RobotMapUp();
    UFUNCTION()
    void RobotMapDown();
	UFUNCTION()
	void RobotStart();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent *CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ARobotControl *Robot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<ARobotControl *> lists;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool RFlag = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TurnView();

	bool RunFlag = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};