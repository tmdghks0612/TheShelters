// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RoomActor.generated.h"

UCLASS()
class THESHELTERS_API ARoomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomActor();

	UFUNCTION(BlueprintCallable)
	void RoomMeshRandomize();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomMesh")
	UStaticMeshComponent* RoomMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomLight")
	UPointLightComponent* RoomPointlightTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomLight")
	TArray<UPointLightComponent*> RoomPointlights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomMesh")
    TArray<UStaticMesh *> RoomMeshVersions;

};
