// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterBase.h"

// Sets default values
AFighterBase::AFighterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkinnedMesh = CreateDefaultSubobject<USkinnedMeshComponent>(TEXT("Mesh"));

	RootComponent = SkinnedMesh;

}

// Called when the game starts or when spawned
void AFighterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFighterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

