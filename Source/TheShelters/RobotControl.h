// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor.h"
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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
    TArray<bool> RouteMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere);
    TArray<UObject *> MapObject;
    
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
	

	UFUNCTION(BlueprintCallable)
    void GiveAddress(TArray<ASurvivor *> _list);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int startLocation = 0;
    int currentLocation;

	TArray<int> route;
    TArray<int> visited;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
