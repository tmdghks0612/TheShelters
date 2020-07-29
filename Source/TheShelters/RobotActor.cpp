// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotActor.h"

// Sets default values
ARobotActor::ARobotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		UE_LOG(LogTemp, Warning, TEXT("Anim In"));
		RobotSkeletalMeshComponent->SetAnimInstanceClass(ROBOT_ANIM.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Anim Failed"));
	}

	if (RobotSkeletalMeshComponent->GetAnimInstance() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Anim not exist"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Anim exist"));	
	}

	//Animinstance = Cast<URobotAniminstance>(RobotSkeletalMeshComponent->GetAnimInstance());
	
}



// Called when the game starts or when spawned
void ARobotActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARobotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARobotActor::SetMovement(bool _Move)
{
	auto RobotAniminstance = Cast<URobotAniminstance>(RobotSkeletalMeshComponent->GetAnimInstance());
	if (nullptr != RobotAniminstance)
	{
		RobotAniminstance->SetMovement(_Move);
	}
}

int ARobotActor::CheckWorking()
{
	
	return 1;
}

