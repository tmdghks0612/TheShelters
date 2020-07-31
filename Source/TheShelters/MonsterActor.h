// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoomControl.h"
#include "MonsterAnimInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "MonsterActor.generated.h"

UCLASS()
class THESHELTERS_API AMonsterActor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterActor();

	UFUNCTION(BlueprintCallable)
	void ChargePanicRoom();

	void RestoreAngry();
	void ActiveAngry();

	UFUNCTION(BlueprintCallable)
	void EnterPanicRoom();

	UFUNCTION(BlueprintCallable)
	bool IsDoorOpen();

	UFUNCTION(BlueprintCallable)
	void StopCharge();
	
	UFUNCTION(BlueprintCallable)
	int GetMonsterId();

	void InitMonsterActor(class ARoomControl* _roomControl, int _monsterId, int _monsterType);
	void MoveTo(FVector destination);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ARoomControl* roomControl;

	int monsterId;
	int monsterType;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterRoot")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterActor")
	USkeletalMesh* MonsterSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MonsterSkeletalMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAngry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCharge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float velocity = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector chargeDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector destination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector chargeLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMonsterAnimInstance* MonsterAnimInstance;
};
