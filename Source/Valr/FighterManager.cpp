// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"
#include "GameFramework/PlayerController.h"

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
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetViewTarget(Camera);
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	if (!Player1 || !Player2 || !Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("FUCK!"));
		return;
	}
	if (Player1->InputID > INPUT::D) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit!"));
	}
		//else UE_LOG(LogTemp, Warning, TEXT("Nah!"));
}

