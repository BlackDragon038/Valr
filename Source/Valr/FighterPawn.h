// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 blockStunRate = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 blockPushPower = 1;
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

	enum STATE { Idle, Moving, Blocking, Attacking, Stunned, Stepping };

	enum KEY_STATE { RESET, RELEASED, PRESSED_ONCE, PRESSED_TWICE };

	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;


	UPROPERTY(BlueprintReadOnly)
		uint8 InputID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter Base Variables")
		uint8 Health = 255;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter Base Variables")
		uint8 MovementSpeed = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter Base Variables")
		uint8 sideStepSpeed = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter Base Variables")
		uint8 Stamina = 100;
	UPROPERTY(BlueprintReadOnly)
		uint8 State;

	uint8 currentFrameOfAttack = 0;
	uint8 currentPartsIndex = 0;
	uint8 stunPush = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data")
		TArray<FAttackData> Attacks;

	TArray<uint8> inputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block Data")
		FBlockData BlockData;

	UPROPERTY(BlueprintReadOnly)
		uint8 attackType = 4;

	uint8 steppingSpeed = 0;
	uint8 steppingFrameTime = 0;
	bool bDoubleTapW = false;
	bool bDoubleTapA = false;
	bool bDoubleTapS = false;
	bool bDoubleTapD = false;
	uint8 KeyW = 0;
	uint8 KeyA = 0;
	uint8 KeyS = 0;
	uint8 KeyD = 0;

	bool UP_Key = 0;
	bool LEFT_Key = 0;
	bool DOWN_Key = 0;
	bool RIGHT_Key = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void AxisW(float Axis);
	void AxisA(float Axis);
	void AxisS(float Axis);
	void AxisD(float Axis);
	void PressedW();
	void PressedA();
	void PressedS();
	void PressedD();
	void ReleasedW();
	void ReleasedA();
	void ReleasedS();
	void ReleasedD();
	void PressedLight();
	void PressedMedium();
	void PressedHeavy();
	void PressedSpecial();
	void PressedBlock();
	void ReleasedBlock();
	void AxisBlock(float Axis);
};
