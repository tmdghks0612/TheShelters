// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotPawn.h"

// Sets default values
ARobotPawn::ARobotPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

	if (!RobotSkeletalMesh) {

		UE_LOG(LogTemp, Warning, TEXT("Robot SkeletalMesh Assigned"));

		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		RobotSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotSkeletalMesh"));
		RobotSkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);

		RobotAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("RobotAudio"));
		RobotAudioComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Assigned RobotSkeletalMesh"));
	}
	
	RobotSkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ROBOT_ANIM(TEXT("/Game/RobotBP/RobotAnimationBP.RobotAnimationBP_C"));
	if (ROBOT_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("ROBOT_ANIM Succeed"));
		RobotSkeletalMeshComponent->SetAnimInstanceClass(ROBOT_ANIM.Class);
		//RobotSkeletalMeshComponent->SetAnimClass(ROBOT_ANIM.Class);
	}
	Destination = GetActorLocation();
	speed = 1000;
	UpdateDestinationFlag = false;
}

// Called when the game starts or when spawned
void ARobotPawn::BeginPlay()
{
	Super::BeginPlay();
	isMoving = false;
	RobotAnimInstance = Cast<URobotAniminstance>(RobotSkeletalMeshComponent->GetAnimInstance());
	Destination = GetActorLocation();
}

// Called every frame
void ARobotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Dir = Destination - GetActorLocation();
	double len = sqrt(Dir.X * Dir.X + Dir.Y * Dir.Y + Dir.Z * Dir.Z);
	if (len > 15)
	{
		UpdateDestinationFlag = false;
		temp = Dir;
		temp.Normalize();
		temp = temp * speed * DeltaTime + GetActorLocation();
		SetActorLocation(temp);
		//SetActorRotation(temp.Rotation().Quaternion());
	}
	else
	{
		if (isMoving && UpdateDestinationFlag == false)
		{
			UpdateDestinationFlag = true;
			RobotControl->StartMoving();
		}
	}
}


// Called to bind functionality to input
void ARobotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// play sound for RobotPawn. 0 for departure, 1 for complete
void ARobotPawn::PlaySound(int soundNum)
{
	switch (soundNum) {
	case 0:

		RobotAudioComponent->SetSound(RobotDepartureSound);
		RobotAudioComponent->Play();
		break;
	case 1:

		RobotAudioComponent->SetSound(RobotCompleteSound);
		RobotAudioComponent->Play();
		break;
	}
}

void ARobotPawn::SetDestination(FVector _Destination)
{
	Destination = _Destination;
	Dir = Destination - GetActorLocation();
	FRotator left(0, 90, 0);
	FRotator right(0, -90, 0);
	FRotator front(0, 0, 0);
	FRotator back(0, 180, 0);
	if (Dir.X < -500)
	{
		SetActorRotation(left);
	}
	else if (Dir.X > 500)
	{
		SetActorRotation(right);
	}
	else if (Dir.Y < -500)
	{
		SetActorRotation(back);
	}
	else if (Dir.Y > 500)
	{
		SetActorRotation(front);
	}
}

void ARobotPawn::SetMovement(bool _Move)
{

	if (nullptr != RobotAnimInstance)
	{
		RobotAnimInstance->SetMovement(_Move);
	}
	isMoving = _Move;
}

void ARobotPawn::SetArrival(bool _isArrived)
{
	if (RobotAnimInstance != nullptr)
	{
		RobotAnimInstance->SetArrival(_isArrived);
	}
}

void ARobotPawn::SetRobotControl(ARobotControl* _RobotControl)
{
	RobotControl = _RobotControl;
}

bool ARobotPawn::GetMovement()
{
	return isMoving;
}