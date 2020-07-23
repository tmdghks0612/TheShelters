// Fill out your copyright notice in the Description page of Project Settings.


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
}

void ASurvivor::TurnView() {
	if (!Controller) {
		UE_LOG(LogTemp, Warning, TEXT("no controller!"));
		return;
	}
	
	AddControllerYawInput(360.0f);

	return;
}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TurnView", IE_Pressed, this, &ASurvivor::TurnView);
    PlayerInputComponent->BindAction("CameraChangeToMap", IE_Pressed, this, &ASurvivor::InitiateMap);

	PlayerInputComponent->BindAction("MapRight", IE_Pressed, this, &ASurvivor::RobotMapRight);
    PlayerInputComponent->BindAction("MapLeft", IE_Pressed, this, &ASurvivor::RobotMapLeft);
    PlayerInputComponent->BindAction("MapUp", IE_Pressed, this, &ASurvivor::RobotMapUp);
    PlayerInputComponent->BindAction("MapDown", IE_Pressed, this, &ASurvivor::RobotMapDown);
	PlayerInputComponent->BindAction("Enter", IE_Pressed, this, &ASurvivor::RobotStart);
}

//Change Camera angle to infront of map + initiate flag for mapping
void ASurvivor::InitiateMap()
{
	RFlag = true;
}

//finish deciding route. 
void ASurvivor::RobotStart()
{
	RFlag = false;
	Robot->SetMove();
}

void ASurvivor::InitRobots(ARobotControl *_Robot)
{
	Robot = _Robot;
}

void ASurvivor::RobotMapRight()
{
	if(RFlag == true)
		Robot->MapRight();
}

void ASurvivor::RobotMapLeft()
{
	if(RFlag == true)
		Robot->MapLeft();
}

void ASurvivor::RobotMapUp()
{
	if(RFlag == true)
		Robot->MapUp();
}

void ASurvivor::RobotMapDown()
{
	if(RFlag == true)
		Robot->MapDown();
}