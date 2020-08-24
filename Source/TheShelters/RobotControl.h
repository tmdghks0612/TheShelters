// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Monster.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Survivor.h"
#include "RobotAniminstance.h"
#include "RobotPawn.h"
#include "LevelControl.h"
#include "GameFramework/Actor.h"
#include "RobotControl.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPrintUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUIDelegate);

UCLASS()
class THESHELTERS_API ARobotControl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotControl();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<bool> RouteMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<UObject *> MapObject;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ARobotPawn> RobotSpawn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ARobotPawn* Robot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int CurrentIndex = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool ToDestination;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool isMoving = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ALevelControl* LevelControl;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int> visited;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URobotAniminstance* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <AMonster*> Monsters;
		

	//Get Input From survivor and make route for robot to move
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
    
	//code to check internal map. Not for gamer
	UFUNCTION()
    void PrintMap();
	
	//general robot movement and search function
	UFUNCTION()
	void RobotMoveTo(int RoomIndex);
	UFUNCTION()
	void ReachDestination();
	UFUNCTION()
	void EndMovement();
	UFUNCTION()
	void SetMove();
	UFUNCTION()
	void DetectMonster();
	UFUNCTION(BlueprintCallable)
	bool StartMoving();
	UFUNCTION()
	void ResourceSearch(int RoomId);

	//survivor, monster, LevelControl finder
	UFUNCTION(BlueprintCallable)
	void FindLevelControl(ALevelControl* _LevelControl);
	UFUNCTION(BlueprintCallable)
    void GiveAddress(ASurvivor* _Survivor);
	UFUNCTION(BlueprintCallable)
	void GetMonsters(TArray<AMonster*> _Monsters);
	UFUNCTION(BlueprintCallable)
	TArray<int> GetVisited();
	UFUNCTION(BlueprintCallable)
	void UIUpdate();
	
	UPROPERTY(BlueprintAssignable, Category = "RobotUI")
	FPrintUIDelegate PrintUI;

	UPROPERTY(BlueprintAssignable, Category = "RobotUI")
	FUpdateUIDelegate UpdateUI;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int startLocation = 5; // panic room location
    int currentLocation;

	Resource RoomResources;
	Resource LoadedResources;
	Resource PanicRoomResources;
	UPROPERTY()
	TArray<int> SearchData;
	UPROPERTY()
	TArray<int> route;
	float startX = 4000.0f;
	float startY = 0.0f;
	float startZ = 200.0f;
	float interval = 1400.0f;
	bool MonsterCheck;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
