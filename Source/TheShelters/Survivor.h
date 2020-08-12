// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelControl.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

/* << ASurvivor : ACharacter >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitSurvivorStat
 * Property:
 * - Hunger
 * - Thirst
 * - Mental
 * - Progress
 *
 * - RFlag
 *
 * Description:
 * Manages Thirst, Hunger every turn.
 * Thirst, Hunger affects Mentality being calculated every turn on EndTurn()
 * Mental stat is not shown to Survivor but it will trigger some events.
 * 
 * RFlag controls whether user is able to control robot.
 * RFlag will be triggered when user enters robot control state.
 */

UCLASS()
class THESHELTERS_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivor();

	UFUNCTION(BlueprintCallable)
	void InitiateMap();

	// Methods for playing game
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	// robot control functions
	UFUNCTION(BlueprintCallable)
	void InitRobots(ARobotControl *_Robot);
	UFUNCTION(BlueprintCallable)
	void FindLevelControl(ALevelControl* _LevelControl);
	UFUNCTION(BlueprintCallable)
	bool ConsumeFood();
	UFUNCTION(BlueprintCallable)
	bool ConsumeWater();
	UFUNCTION(BlueprintCallable)
	float GetHunger(); 
	UFUNCTION(BlueprintCallable)
	float GetThirst();
	UFUNCTION(BlueprintCallable)
	float GetMental();


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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ALevelControl* LevelControl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool RFlag = false;


	// Getters
	const int Hunger() const;
	const int Thirst() const;
	const double Mental() const;


	// Setters
	const int Hunger(const int diff);
	const int Thirst(const int diff);
	const double Mental(const double diff);

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

	void MoveForward(float amount);
	void MoveRight(float amount);
	void LookUp(float amount);
	void Turn(float amount);

private:
	int maxHunger = 100;
	int maxThirst = 100;
	int maxMental = 100;
	
	int hungerRestoreAmount = 5;
	int thirstRestoreAmount = 5;

	int hungerPerTurn = 1;
	int thirstPerTurn = 2;

	int hunger;
	int thirst;
	double mental;

	int progress;

	// Private methods for playing game
	void ResourceNeedPerTurn();
	const double MentalMultiplier() const;
};