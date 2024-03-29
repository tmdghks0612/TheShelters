// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
#include "RobotControl.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#define StatSetter(x, y, z)                                                                                            \
    {                                                                                                                  \
        (x) += (y);                                                                                                    \
        (x) = std::max(0, (x));                                                                                        \
        (x) = std::min((x), (z));                                                                                      \
    }
// Sets default values
ASurvivor::ASurvivor()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

    SpringArmComp->SetupAttachment(RootComponent);
    // using default arm length
    // SpringArmComp->TargetArmLength = 400;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);

	SurvivorSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SurvivorSkeletalMeshComponent->SetupAttachment(RootComponent);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
    MeshComp->SetupAttachment(RootComponent);

	SurvivorAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("RobotAudio"));
	SurvivorAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

    BaseTurnRate = 45.0f;
    RFlag = false;

	currentMentalState = MentalState::NORMAL;

	hunger = 50;
	thirst = 50;
	mental = 100;
}

// Called when the game starts or when spawned
void ASurvivor::BeginPlay()
{
    Super::BeginPlay();
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
    PlayerInputComponent->BindAction("CameraChangeToMap", IE_Pressed, this, &ASurvivor::InitiateMap);

    PlayerInputComponent->BindAction("MapRight", IE_Pressed, this, &ASurvivor::RobotMapRight);
    PlayerInputComponent->BindAction("MapLeft", IE_Pressed, this, &ASurvivor::RobotMapLeft);
    PlayerInputComponent->BindAction("MapUp", IE_Pressed, this, &ASurvivor::RobotMapUp);
    PlayerInputComponent->BindAction("MapDown", IE_Pressed, this, &ASurvivor::RobotMapDown);
    PlayerInputComponent->BindAction("Enter", IE_Pressed, this, &ASurvivor::RobotStart);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivor::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivor::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASurvivor::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ASurvivor::Turn);
}

void ASurvivor::MoveForward(float amount)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), amount);
}

void ASurvivor::MoveRight(float amount)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), amount);
}

void ASurvivor::LookUp(float amount)
{
	AddControllerPitchInput(amount);
}

void ASurvivor::Turn(float amount)
{
	AddControllerYawInput(amount);
}

// resource managing functions

// restore hunger / thirst on consuming food / water
bool ASurvivor::ConsumeFood()
{
	int current_food = LevelControl->GetPanicRoomFood();
	if (current_food > 0) {
		LevelControl->SetPanicRoomFood(current_food - 1);
		Hunger(hungerRestoreAmount);
		return true;
	}
	else {
		return false;
	}
}

bool ASurvivor::ConsumeWater()
{
	int current_water = LevelControl->GetPanicRoomWater();
	if (current_water > 0) {
		LevelControl->SetPanicRoomWater(current_water - 1);
		Thirst(thirstRestoreAmount);
		return true;
	}
	else {
		return false;
	}
}

float ASurvivor::GetHunger()
{
	return (float)hunger / maxHunger * 1.0f;;
}

float ASurvivor::GetThirst()
{
	return (float)thirst / maxThirst * 1.0f;
}

float ASurvivor::GetMental()
{
	return (float)mental / maxMental * 1.0f;
}

// hunger, thirst intensify every turn
void ASurvivor::ResourceNeedPerTurn()
{
	Hunger(-hungerPerTurn);
	Thirst(-thirstPerTurn);
}

// called on GameControl EndTurn to change survivor's stat every turn
void ASurvivor::EndTurn()
{
	double mentalMultiplier = MentalMultiplier();
	// Consume food and water
	ResourceNeedPerTurn();

	// Change mental
	double mentalDiff = 1 * mentalMultiplier;
	Mental(mentalDiff);
	UE_LOG(LogTemp, Warning, TEXT("mental : %f"), mental);
	if (mental >= mentalThreshold3) {
		if (currentMentalState != MentalState::NORMAL) {
			SurvivorAudioComponent->Stop();
			SurvivorAudioComponent->SetSound(HeartbeatSound3);
			SurvivorAudioComponent->Play();
			currentMentalState = MentalState::NORMAL;
			ChangeMentalStateEvent.Broadcast((uint8)currentMentalState);
		}
	}
	else if (mental >= mentalThreshold2) {
		if (currentMentalState != MentalState::UNSTABLE) {
			SurvivorAudioComponent->Stop();
			SurvivorAudioComponent->SetSound(HeartbeatSound2);
			SurvivorAudioComponent->Play();
			currentMentalState = MentalState::UNSTABLE;
			ChangeMentalStateEvent.Broadcast((uint8)currentMentalState);
		}
	}
	else if (mental >= mentalThreshold1) {
		if (currentMentalState != MentalState::DYING) {
			SurvivorAudioComponent->Stop();
			SurvivorAudioComponent->SetSound(HeartbeatSound1);
			SurvivorAudioComponent->Play();
			currentMentalState = MentalState::DYING;
			ChangeMentalStateEvent.Broadcast((uint8)currentMentalState);
		}
	}
	else if (mental <= 0) {
		if (!LevelControl->IsGameOver) {
			LevelControl->GameOver();
		}
	}
}

// calculates mental multiplier according to current hunger and thirst
const double ASurvivor::MentalMultiplier() const
{
	double hungerMultiplier, thirstMultiplier;
	if (this->hunger < 30)
	{
		hungerMultiplier = (std::log(this->hunger + 1) / std::log(31.0)) - 2;
	}
	else if (this->hunger > 60)
	{
		hungerMultiplier = std::log(this->hunger - 59) / std::log(41) + 1;
	}
	else
	{
		hungerMultiplier = (this->hunger / 15.0) - 3;
	}

	if (this->thirst < 30)
	{
		thirstMultiplier = (std::log(this->thirst + 1) / std::log(31.0)) - 2;
	}
	else if (this->thirst > 60)
	{
		thirstMultiplier = std::log(this->thirst - 59) / std::log(41) + 1;
	}
	else
	{
		thirstMultiplier = (this->thirst / 15.0) - 3;
	}

	return round((hungerMultiplier + thirstMultiplier) * 100.0) / 100.0;
}

// Getter
const int ASurvivor::Hunger() const
{
	return this->hunger;
}

const int ASurvivor::Thirst() const
{
	return this->thirst;
}

const double ASurvivor::Mental() const
{
	return this->mental;
}


// Setters
const int ASurvivor::Hunger(const int diff)
{
	StatSetter(hunger, diff, maxHunger);
	return hunger;
}

const int ASurvivor::Thirst(const int diff)
{
	StatSetter(thirst, diff, maxThirst);
	return thirst;
}

const double ASurvivor::Mental(const double diff)
{
	mental += diff;
	mental = std::max(0.0, mental);
	mental = std::min(mental, static_cast<double>(maxMental));
	
	return mental;
}


// Change Camera angle to infront of map + initiate flag for mapping
void ASurvivor::InitiateMap()
{
    if(RFlag == false && RunFlag == false && LevelControl->UIShowFlag)
        RFlag = true;
}

// finish deciding route.
void ASurvivor::RobotStart()
{
	RFlag = false;
	Robot->SetMove();
}

void ASurvivor::InitRobots(ARobotControl *_Robot)
{
    Robot = _Robot;
}

void ASurvivor::FindLevelControl(ALevelControl * _LevelControl)
{
	LevelControl = _LevelControl;
}

void ASurvivor::RobotMapRight()
{
    if (RFlag == true && LevelControl->UIShowFlag)
        Robot->MapRight();
}

void ASurvivor::RobotMapLeft()
{

    if (RFlag == true && LevelControl->UIShowFlag)
        Robot->MapLeft();
}

void ASurvivor::RobotMapUp()
{

    if (RFlag == true && LevelControl->UIShowFlag)
        Robot->MapUp();
}

void ASurvivor::RobotMapDown()
{

    if (RFlag == true && LevelControl->UIShowFlag)
        Robot->MapDown();
}
