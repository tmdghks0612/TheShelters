// Fill out your copyright notice in the Description page of Project Settings.


#include "PanicRoomDoor.h"

APanicRoomDoor::APanicRoomDoor()
{
	if (!DoorSkeletalMesh) 
	{
		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;

		DoorSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorSkeletalMesh"));
		DoorSkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
	}

	DoorSkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> DOOR_ANIM(TEXT("/Game/DoorBP/DoorAnimationBP.DoorAnimationBP_C"));
	if (DOOR_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Anim In"));
		DoorSkeletalMeshComponent->SetAnimInstanceClass(DOOR_ANIM.Class);
	}
}

void APanicRoomDoor::SetDoor()
{
	auto DoorAnimInstance = Cast<UDoorAnimInstance>(DoorSkeletalMeshComponent->GetAnimInstance());
	if (nullptr != DoorAnimInstance)
	{
		DoorAnimInstance->SetDoor();
	}
}


