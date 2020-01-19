// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include <vector>

#include "FighterPawn.generated.h"

USTRUCT(BlueprintType)
struct FFrameData {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 hold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 psum;
};

USTRUCT(BlueprintType)
struct FAttackData {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		uint8 damage;
	UPROPERTY(EditAnywhere)
		TArray<FFrameData> frames;
};

UCLASS()
class VALR_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(BlueprintReadWrite)
	uint8 i;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 stamina;
	enum state { idle, moving, blocking, attacking, stunned, recovering };
	uint8 lock_frames;
	UPROPERTY(EditAnywhere)
		TArray<FAttackData> attacks;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void PressedW();
	void PressedA();
	void PressedS();
	void PressedD();
	void ReleasedW();
	void ReleasedA();
	void ReleasedS();
	void ReleasedD();
	void Pressed1();
	void Pressed2();
	void Pressed3();
	void Pressed4();
	/*void Pressed5();
	void Pressed6();
	void Pressed7();
	void Pressed8();
	void Pressed9();
	void Pressed10();
	void Pressed11();
	void Pressed12();*/
};
