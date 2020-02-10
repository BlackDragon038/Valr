// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Samurai.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ASamurai : public AFighterPawn
{
	GENERATED_BODY()

		void Tick(float DeltaTime);

		void PressedSpecial() override;
	
};
