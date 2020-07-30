// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotPawn.h"

// Sets default values
ARobotPawn::ARobotPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RobotSkeletalMesh) {

		UE_LOG(LogTemp, Warning, TEXT("Robot SkeletalMesh Assigned"));

		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		RobotSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotSkeletalMesh"));
		RobotSkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
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
}

// Called when the game starts or when spawned
void ARobotPawn::BeginPlay()
{
	Super::BeginPlay();
	isMoving = false;
	RobotAnimInstance = Cast<URobotAniminstance>(RobotSkeletalMeshComponent->GetAnimInstance());
}

// Called every frame
void ARobotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARobotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

bool ARobotPawn::GetMovement()
{
	return isMoving;
}