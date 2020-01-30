// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include <vector>
#include "FighterPawn.generated.h"

USTRUCT(BlueprintType)
struct FBlockData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Angle;
};
 
USTRUCT(BlueprintType)
struct FFrameData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Hold = 1;
		uint8 PSum = 0;
};

USTRUCT(BlueprintType)
struct FAttackData 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		uint8 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 StunRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 StunPushPower = 5;
	UPROPERTY(EditAnywhere)
		TArray<FFrameData> Parts;

	UPROPERTY(BlueprintReadOnly)
	int AttackTotalFrameCount = 0;
};

UCLASS()
class VALR_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	enum INPUT { IDLE, UP, UP_LEFT, LEFT, LEFT_DOWN, DOWN, DOWN_RIGHT, RIGHT, RIGHT_UP, Light, Medium, Heavy, Special, Block };

	enum ATTACK_TYPE { LIGHT, MEDIUM, HEAVY, SPECIAL, NONE};

	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;


	UPROPERTY(BlueprintReadWrite)
		uint8 InputID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isFirstPlayer = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Health = 255;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MovementSpeed = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Stamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 blockStunRate = 25;

	enum STATE { Idle, Moving, Blocking, Attacking, Stunned };

	UPROPERTY(BlueprintReadWrite)
		uint8 State;

	uint8 currentFrameOfAttack = 0;
	uint8 currentPartsIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FAttackData> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FBlockData BlockData;

	UPROPERTY(BlueprintReadOnly)
	uint8 attackType = 4;

	int8 AxisW = 0;
	int8 AxisA = 0;
	int8 AxisS = 0;
	int8 AxisD = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void PressedW(float Axis);
	void PressedA(float Axis);
	void PressedS(float Axis);
	void PressedD(float Axis);
	void PressedLight();
	void PressedMedium();
	void PressedHeavy();
	void PressedSpecial();
	void PressedBlock();
	void ReleasedBlock();
};
