// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Crusader.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ACrusader : public AFighterPawn
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* specialAttackParticle;

	void BeginPlay();
	
	void Tick(float DeltaTime);

	void PressedSpecial() override;
	
};
