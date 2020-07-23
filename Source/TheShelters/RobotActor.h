// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotAniminstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotActor.generated.h"

UCLASS()
class THESHELTERS_API ARobotActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URobotAniminstance* Animinstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	int CheckWorking();

	UFUNCTION()
	void SetMovement(bool _Move);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotActor")
	USkeletalMesh* RobotSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* RobotSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimBlueprint* AnimationBP;


};
