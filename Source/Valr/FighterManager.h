// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "GameFramework/Actor.h"

#include "FighterPawn.h"

#include "FighterManager.generated.h"

UCLASS()
class VALR_API AFighterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFighterManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFighterPawn* Player1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFighterPawn* Player2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACamera* Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 roundTimer = 3600;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Angle(FVector a, FVector b);

	bool bPlayer1IsHit = false;
	bool bPlayer2IsHit = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
