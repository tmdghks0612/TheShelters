// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.fwd.h"
#include "LevelControl.fwd.h"

#include "Direction.h"
#include "MonsterAnimInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Monster.generated.h"

// MonsterType determines MonsterProperty values and movement speed
UENUM() enum class MonsterType { Ghoul, Grima };

// MonsterProperty <radioactive, emp, armorpierce>
typedef TTuple<bool, bool, bool> MonsterProperty;

/* << AMonster : APawn>>
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
class THESHELTERS_API AMonster : public APawn

{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AMonster();

    // Initializers
	void InitMonster(MonsterType t, int _monsterId);
	void InitMonsterActor(class ALevelControl* _LevelControl, int _monsterId, MonsterType _monsterType);
	void MoveTo(FVector destination);

    // PanicRoom related methods
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMonsterAnimInstance *MonsterAnimInstance;

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

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  private:
    ALevelControl *LevelControl;
    // Default monster values
    int monsterId;
    MonsterType monsterType;

    // Monster properties
    bool radioactive;
    bool emp;
    bool armorpierce;

    // Monster movement speed
    int speed;

    Direction prevDirection;
};
