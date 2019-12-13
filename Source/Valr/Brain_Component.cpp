// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain_Component.h"
#include "Engine/Engine.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Public/GenericPlatform/GenericPlatformMath.h"

// Sets default values for this component's properties
UBrain_Component::UBrain_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrain_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ann = new ArtificialNN(3, 2, 1, 6, 0.2f);
}


// Called every frame
void UBrain_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

