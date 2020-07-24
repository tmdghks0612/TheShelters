// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoomControl.h"

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

	UFUNCTION(BlueprintCallable)
	void ChargePanicRoom();

	void RestoreAngry();

	UFUNCTION(BlueprintCallable)
	void EnterPanicRoom();

	UFUNCTION(BlueprintCallable)
	bool IsDoorOpen();

	void InitMonsterActor(class ARoomControl* _roomControl, int _monsterId);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ARoomControl* roomControl;

	int monsterId;

public:	
	DECLARE_EVENT(AMonsterActor, FonAngry)

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter")
	void onAngry();

	DECLARE_EVENT(AMonsterActor, FonCancelAngry)

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter")
	void onCancleAngry();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterActor")
	USkeletalMesh* MonsterSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MonsterSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomMesh")
	TArray<UAnimBlueprint *> AnimationBPs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAngry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector chargeDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector panicRoomLocation;

};
