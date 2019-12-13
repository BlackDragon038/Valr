// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkLayer.h"

NetworkLayer::NetworkLayer(int nNeurons, int numNeuronInputs)
{
	numNeurons = nNeurons;
	for (int i = 0; i < nNeurons; i++)
	{
		neurons.Add(new Neuron(numNeuronInputs));
	}
}

NetworkLayer::~NetworkLayer()
{
	//for (int i = neurons.Num() - 1; i >= 0; i--)
	//{
	//	delete neurons[i];
	//}
}
