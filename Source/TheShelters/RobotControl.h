// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor.h"
#include "RobotActor.h"
#include "GameFramework/Actor.h"
#include "RobotControl.generated.h"

UCLASS()
class THESHELTERS_API ARobotControl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotControl();

	UFUNCTION(BlueprintCallable)
    void RobotRouteSelect(TArray<UObject *> ShelterMap);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<bool> RouteMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<UObject *> MapObject;
    
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSubclassOf<class ARobotActor> RobotSpawn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ARobotActor* Robot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int CurrentIndex = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool ToDestination;

	UFUNCTION()
	void MapRight();
    UFUNCTION()
    void MapLeft();
    UFUNCTION()
    void MapUp();
    UFUNCTION()
    void MapDown();
    UFUNCTION()
    void initMap();
    UFUNCTION()
    void PrintMap();
	UFUNCTION()
	void StartMoving();
	UFUNCTION()
	void RobotMoveTo(int RoomIndex);
	UFUNCTION()
	void ReachDestination();
	UFUNCTION()
	void EndMovement();
	
	UFUNCTION(BlueprintCallable)
    void GiveAddress(TArray<ASurvivor*> _List);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int startLocation = 0;
    int currentLocation;

	TArray<int> route;
    TArray<int> visited;

	//ARobotActor* Robot;
	//monsterActors.Add(world->SpawnActor<AMonsterActor>(MonsterSpawn[0], spawnLocation, rotator, spawnParams))

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
