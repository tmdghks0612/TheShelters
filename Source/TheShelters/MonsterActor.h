// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterActor.fwd.h"
#include "RoomControl.fwd.h"

#include "Direction.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "MonsterActor.generated.h"

// MonsterType determines MonsterProperty values and movement speed
enum MonsterType
{
	DefaultMonster
};

// MonsterProperty <radioactive, emp, armorpierce>
typedef TTuple<bool, bool, bool> MonsterProperty;

/* << AMonsterActor : AActor >>
 * Constructor:
 * - Default Constructor
 * Initializer:
 * - InitMonster: MonsterType, MonsterId
 * Property:
 * - ID
 * - Type
 * - Speed
 * - Radioactive
 * - Emp
 * - Armorpierce
 *
 * Description:
 * A monster is just a data class. Every behavior is delegated to ShelterControl.
 * Monsters have ID, which is unique. It has type, which determines monster's
 * all property values. Monsters have speed, which determines maximum distance
 * to move for each turn. And monsters have three properties. Radioactive spoil
 * nearby food and water. Emp break nearby CCTVs and doors up. Armorpiercing
 * monster can break armor up and give fatal damage to the player.
 */
UCLASS()
class THESHELTERS_API AMonsterActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterActor();

	// Initializers
	void InitMonsterActor(ARoomControl *_roomControl, int _monsterId);
	void InitMonster(MonsterType t, int id);

	// PanicRoom related methods
	UFUNCTION(BlueprintCallable)
	void ChargePanicRoom();
	UFUNCTION(BlueprintCallable)
	void EnterPanicRoom();
	UFUNCTION(BlueprintCallable)
	bool IsDoorOpen();
	void RestoreAngry();

	// Event related methods
	DECLARE_EVENT(AMonsterActor, FonAngry)
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter")
	void OnAngry();
	DECLARE_EVENT(AMonsterActor, FonCancelAngry)
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter")
	void OnCancleAngry();

	// Getters and Setters
	const int MonsterId() const;
	const MonsterType Type() const;
	const int Speed() const;
	const MonsterProperty Property() const;
	const Direction PreviousDirection() const;
	void PreviousDirection(Direction d);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterRoot")
	USceneComponent *Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterActor")
	USkeletalMesh *MonsterSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *MonsterSkeletalMeshComponent;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ARoomControl *roomControl;
	// Default monster values
	int id;
	MonsterType type;

	// Monster properties
	bool radioactive;
	bool emp;
	bool armorpierce;

	// Monster movement speed
	int speed;

	Direction prevDirection;
};
