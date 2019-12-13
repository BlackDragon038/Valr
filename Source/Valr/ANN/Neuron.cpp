// Fill out your copyright notice in the Description page of Project Settings.

#include "Neuron.h"

Neuron::Neuron(int nInputs)
{
	float weightRange = 2.4f / (nInputs * 1.0f);
	bias = FMath::RandRange(-weightRange, weightRange);
	numInputs = nInputs;

	for (int i = 0; i < nInputs; i++)
		weights.Add(FMath::RandRange(-weightRange, weightRange));
}

Neuron::~Neuron()
{
}
