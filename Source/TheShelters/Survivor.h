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
	const int Food() const;
	const int Water() const;
	const int Mental() const;
	const int Progress() const;
	const int Electricity() const;

	const int Food(const int diff);
	const int Water(const int diff);
	const int Mental(const int diff);
	const int Progress(const int diff);
	const int Electricity(const int diff);

	// Methods for playing game
	void TimePass();

private:
	int max = 100;
	int food;
	int water;
	int mental;
	int progress;
	int electricity;

	int mentalMultiplier;
	int electricityUsage;

	// Private methods for playing game
	void Consume();
}
