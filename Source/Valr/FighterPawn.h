// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include <vector>
#include "FighterPawn.generated.h"

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
		uint8 Hold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PSum;
};

USTRUCT(BlueprintType)
struct FAttackData 
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		uint8 Damage;
	UPROPERTY(EditAnywhere)
		TArray<FFrameData> Frames;
};

UCLASS()
class VALR_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	enum INPUT { IDLE, UP, UP_LEFT, LEFT, LEFT_DOWN, DOWN, DOWN_RIGHT, RIGHT, RIGHT_UP, Light, Medium, Heavy, Special };

	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 InputID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isFirstPlayer = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Stamina = 100;
	enum STATE { Idle, Moving, Blocking, Attacking, Stunned, Recovering };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		uint8 State;

	uint8 lockFrames;

	UPROPERTY(EditAnywhere)
		TArray<FAttackData> Attacks;

	int8 AxisW = 0, AxisA = 0, AxisS = 0, AxisD = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector currentVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Angle;

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
};
