// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera.h"
#include "FighterPawn.h"

#include "FightManager.generated.h"

UENUM(BlueprintType)
enum class ROUND_STATE : uint8 { ROUND_OVER_PLAYER1_WINS, ROUND_OVER_PLAYER2_WINS, ROUND_RESTARTING, ROUND_ABOUT_TO_START, ROUND_ONGOING };

UCLASS()
class VALR_API AFightManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFightManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFighterPawn* Player1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFighterPawn* Player2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACamera* Camera = nullptr;

	UPROPERTY(BlueprintReadOnly)
		int32 roundTimer = 3600;

	UPROPERTY(BlueprintReadOnly)
		int32 timeToStart = 180;

	UPROPERTY(BlueprintReadOnly)
		uint8 roundCount = 1;

	UPROPERTY(BlueprintReadOnly)
		uint8 Player1Score = 0;

	UPROPERTY(BlueprintReadOnly)
		uint8 Player2Score = 0;

	UPROPERTY(BlueprintReadOnly)
		ROUND_STATE roundState;

	uint8 t = 100;
	uint8 n = 0;

	bool bPlayer1IsHit = false;
	bool bPlayer2IsHit = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void processPlayer(AFighterPawn* &P1, AFighterPawn* &P2, FVector toP1, FVector toP2, bool &bOpponentIsHit);

	void testRoundStatus();

	float Angle(FVector a, FVector b);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
