// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"
#include "DrawDebugHelpers.h"
#include <cmath>
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFighterManager::AFighterManager() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFighterManager::BeginPlay() 
{
	Super::BeginPlay();
	if (!Player1 || !Player2 || !Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("FighterManager: The pointers aren't set up in the Blueprint!"));
		return;
	}
	GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera);
}

float AFighterManager::Angle(FVector a, FVector b)
{
	return FMath::RadiansToDegrees(std::acos(FVector::DotProduct(a, b) / a.Size() * b.Size()));
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

