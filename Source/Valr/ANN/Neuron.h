// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class  Neuron
{
public:
	int numInputs;
	double bias;
	double output;
	double errorGradient;

	TArray<double> weights;
	TArray<double> inputs;

	Neuron(int nInputs);
	~Neuron();
};
