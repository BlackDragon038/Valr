// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	AFighterPawn* p1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFighterPawn* p2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 round_timer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
