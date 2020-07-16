// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "MonsterActor.generated.h"

UCLASS()
class THESHELTERS_API AMonsterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterActor")
	USkeletalMesh* MonsterSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MonsterSkeletalMeshComponent;
};
