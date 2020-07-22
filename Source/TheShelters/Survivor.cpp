// Fill out your copyright notice in the Description page of Project Settings.

#include <algorithm>
#include <cmath>

#include "RobotControl.h"
#include "Survivor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ASurvivor::ASurvivor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	SpringArmComp->SetupAttachment(RootComponent);
	// using default arm length
	//SpringArmComp->TargetArmLength = 400;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	BaseTurnRate = 45.0f;
}

// Called when the game starts or when spawned
void ASurvivor::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerStat();
}

void ASurvivor::TurnView()
{
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("no controller!"));
		return;
	}

	AddControllerYawInput(360.0f);

	return;
}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TurnView", IE_Pressed, this, &ASurvivor::TurnView);
	PlayerInputComponent->BindAction("CameraChangeToMap", IE_Pressed, this, &ASurvivor::CameraChange);

	PlayerInputComponent->BindAction("MapRight", IE_Pressed, this, &ASurvivor::RobotMapRight);
	PlayerInputComponent->BindAction("MapLeft", IE_Pressed, this, &ASurvivor::RobotMapLeft);
	PlayerInputComponent->BindAction("MapUp", IE_Pressed, this, &ASurvivor::RobotMapUp);
	PlayerInputComponent->BindAction("MapDown", IE_Pressed, this, &ASurvivor::RobotMapDown);
}

void ASurvivor::CameraChange()
{
}

void ASurvivor::InitRobots(ARobotControl *_Robot)
{
	Robot = _Robot;
}

void ASurvivor::RobotMapRight()
{
	Robot->MapRight();
}

void ASurvivor::RobotMapLeft()
{
	Robot->MapLeft();
}

void ASurvivor::RobotMapUp()
{
	Robot->MapUp();
}

void ASurvivor::RobotMapDown()
{
	Robot->MapDown();
}

UPlayerStat::UPlayerStat() {}
UPlayerStat::~UPlayerStat() {}

void UPlayerStat::InitPlayerStat()
{
	this->food = 100;
	this->water = 100;
	this->mental = 50;
	this->progress = 0;
	this->electricity = 100;
}

const double UPlayerStat::Food() const
{
	return this->food;
}

const double UPlayerStat::Water() const
{
	return this->water;
}

const double UPlayerStat::Mental() const
{
	return this->mental;
}

const double UPlayerStat::Progress() const
{
	return this->progress;
}

const double UPlayerStat::Electricity() const
{
	return this->electricity;
}

const double UPlayerStat::Food(const double diff)
{
	this->food += diff;
	this->food = std::max(0, this->food);
	this->food = std::min(this->food, this->max);
	return this->food;
}

const double UPlayerStat::Water(const double diff)
{
	this->water += diff;
	this->water = std::max(0, this->water);
	this->water = std::min(this->water, this->max);
	return this->water;
}
const double UPlayerStat::Mental(const double diff)
{
	this->mental += diff;
	this->mental = std::max(0, this->mental);
	this->mental = std::min(this->mental, this->max);
	return this->mental;
}
const double UPlayerStat::Progress(const double diff)
{
	this->progress += diff;
	this->progress = std::max(0, this->progress);
	this->progress = std::min(this->progress, this->max);
	return this->progress;
}
const double UPlayerStat::Electricity(const double diff)
{
	this->electricity += diff;
	this->electricity = std::max(0, this->electricity);
	this->electricity = std::min(this->electricity, this->max);
	return this->electricity;
}

void UPlayerStat::Consume()
{
	double foodPerTime = 1;
	double waterMultiplier = 2;
	Food(-foodPerTime);
	Water(-foodPerTime * waterMultiplier);
}

double UPlayerStat::MentalMultiplier()
{
	double foodMultiplier, waterMultiplier;
	if (this->food < 30)
	{
		foodMultplier = (std::log(this->food + 1) / std::log(31.0)) - 1;
	}
	else if (this->food > 60)
	{
		foodMultiplier = std::log(this->food - 59) / std::log(41);
	}
	else
	{
		foodMultiplier = (this->food / 15) - 3;
	}

	if (this->water < 30)
	{
		waterMultiplier = (std::log(this->water + 1) / std::log(31.0)) - 1;
	}
	else if (this->food > 60)
	{
		waterMultiplier = std::log(this->water - 59) / std::log(41);
	}
	else
	{
		waterMultiplier = (this->water / 15) - 3;
	}

	return foodMultiplier + waterMultiplier;
}

void UPlayerStat::TimePass()
{
	double mentalMultiplier = MentalMupltiplier();
	// Consume food and water
	Consume();

	// Change mental
	double mentalDiff = 1 * mentalMultiplier;
	Mental(mentalDiff);

	// Consume electricity
	Electricity(-electricityUsage);
}