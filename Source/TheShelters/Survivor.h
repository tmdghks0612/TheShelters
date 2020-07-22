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
	void CameraChange();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TurnView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

private:
	unsigned int playerStat;
};

UCLASS()
class THESHELTERS_API UPlayerStat : public UObject
{
	GENERATED_BODY()
public:
	// Constructors and Initializers
	UPlayerStat();
	~UPlayerStat();
	void InitPlayerStat();

	// Getters
	const double Food() const;
	const double Water() const;
	const double Mental() const;
	const double Progress() const;
	const double Electricity() const;

	const double Food(const double diff);
	const double Water(const double diff);
	const double Mental(const double diff);
	const double Progress(const double diff);
	const double Electricity(const double diff);

	// Methods for playing game
	void TimePass();

private:
	double max = 100;
	double food;
	double water;
	double mental;
	double progress;
	double electricity;

	int electricityUsage;

	// Private methods for playing game
	void Consume();
	double MentalMultiplier();
}
