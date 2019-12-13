// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

#include "ANN/ArtificialNN.h"

#include "Brain_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBrain_Component : public UActorComponent
{
	GENERATED_BODY()

private:

	ArtificialNN *ann;

public:	
	// Sets default values for this component's properties
	UBrain_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
