// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Neuron.h"
/**
 * 
 */
class  NetworkLayer
{
public:

	int numNeurons;
	TArray<Neuron *> neurons;


	NetworkLayer(int nNeurons, int numNeuronInputs);
	~NetworkLayer();
};
