// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtificialNN.h"
//#include <math.h>       /* round, floor, ceil, trunc */
#include "Engine.h"

ArtificialNN::ArtificialNN(int nIn, int nOut, int nHidden, int nPH, double a)
{
	numInputs = nIn;
	numOutputs = nOut;
	numHidden = nHidden;
	numNPerHidden = nPH;
	alpha = a;

	if (numHidden > 0)
	{
		layers.Add(new NetworkLayer (numNPerHidden, numInputs));

		for (int i = 0; i < numHidden - 1; i++)
		{
			layers.Add(new NetworkLayer (numNPerHidden, numNPerHidden));
		}

		layers.Add(new NetworkLayer(numOutputs, numNPerHidden));
	}
	else
	{
		layers.Add(new NetworkLayer(numOutputs, numInputs));
	}
}

ArtificialNN::~ArtificialNN()
{
	//for (int i = layers.Num() - 1; i >= 0; i--)
	//{
	//	delete layers[0];
	//}
}

TArray<double> ArtificialNN::Train(TArray<double> inputValues, TArray<double> desiredOutput)
{
	TArray<double> outputValues = TArray<double>();

	outputValues = CalcOutput(inputValues, desiredOutput);
	UpdateWeights(outputValues, desiredOutput);

	return outputValues;
}

TArray<double> ArtificialNN::CalcOutput(TArray<double> inputValues)
{
	TArray<double> inputs;
	TArray<double> outputValues;
	int currentInput = 0;

	if (inputValues.Num() != numInputs)
	{
		//"ERROR: Number of Inputs must be " + numInputs);
		return outputValues;
	}

	inputs = inputValues;
	for (int i = 0; i < numHidden + 1; i++)
	{
		if (i > 0)
		{
			inputs = outputValues;
		}
		outputValues.Empty();

		for (int j = 0; j < layers[i]->numNeurons; j++)
		{
			double N = 0;
			layers[i]->neurons[j]->inputs.Empty();

			for (int k = 0; k < layers[i]->neurons[j]->numInputs; k++)
			{
				layers[i]->neurons[j]->inputs.Add(inputs[currentInput]);
				N += layers[i]->neurons[j]->weights[k] * inputs[currentInput];
				currentInput++;
			}

			N -= layers[i]->neurons[j]->bias;

			if (i == numHidden)
				layers[i]->neurons[j]->output = ActivationFunctionO(N);
			else
				layers[i]->neurons[j]->output = ActivationFunction(N);

			outputValues.Add(layers[i]->neurons[j]->output);
			currentInput = 0;
		}
	}
	return outputValues;
}

TArray<double> ArtificialNN::CalcOutput(TArray<double> inputValues, TArray<double> desiredOutput)
{
	TArray<double> inputs; // = new TArray<double>();
	TArray<double> outputValues; // = new TArray<double>();
	int currentInput = 0;

	if (inputValues.Num() != numInputs)
	{
		//"ERROR: Number of Inputs must be " + numInputs);
		return outputValues;
	}

	inputs = inputValues;
	for (int i = 0; i < numHidden + 1; i++)
	{
		if (i > 0)
		{
			inputs = outputValues;
		}
		outputValues.Empty();
		
		for (int j = 0; j < layers[i]->numNeurons; j++)
		{
			double N = 0;
			layers[i]->neurons[j]->inputs.Empty();

			for (int k = 0; k < layers[i]->neurons[j]->numInputs; k++)
			{
				layers[i]->neurons[j]->inputs.Add(inputs[currentInput]);
				N += layers[i]->neurons[j]->weights[k] * inputs[currentInput];
				currentInput++;
			}

			N -= layers[i]->neurons[j]->bias;

			if (i == numHidden)
				layers[i]->neurons[j]->output = ActivationFunctionO(N);
			else
				layers[i]->neurons[j]->output = ActivationFunction(N);

			outputValues.Add(layers[i]->neurons[j]->output);
			currentInput = 0;
		}
	}
	return outputValues;
}

FString ArtificialNN::PrintWeights()
{
	FString weightStr = "";
	for(NetworkLayer *l : layers)
	{
		for(Neuron *n : l->neurons)
		{
			for(double w : n->weights)
			{
				weightStr += FString::SanitizeFloat(w) + ",";
			}
			weightStr += FString::SanitizeFloat(n->bias) + ",";
		}
	}
	return weightStr;
}

void ArtificialNN::UpdateWeights(TArray<double> outputs, TArray<double> desiredOutput)
{
	double error;
	for (int i = numHidden; i >= 0; i--)	//start from end : backpropagations
	{
		for (int j = 0; j < layers[i]->numNeurons; j++)
		{
			if (i == numHidden)
			{
				error = desiredOutput[j] - outputs[j];
				layers[i]->neurons[j]->errorGradient = outputs[j] * (1 - outputs[j]) * error;
				//errorGradient calculated with Delta Rule: en.wikipedia.org/wiki/Delta_rule
			}
			else
			{
				layers[i]->neurons[j]->errorGradient = layers[i]->neurons[j]->output * (1 - layers[i]->neurons[j]->output);
				double errorGradSum = 0;
				for (int p = 0; p < layers[i + 1]->numNeurons; p++)
				{
					errorGradSum += layers[i + 1]->neurons[p]->errorGradient * layers[i + 1]->neurons[p]->weights[j];
				}
				layers[i]->neurons[j]->errorGradient *= errorGradSum;
			}
			for (int k = 0; k < layers[i]->neurons[j]->numInputs; k++)
			{
				if (i == numHidden)
				{
					error = desiredOutput[j] - outputs[j];
					layers[i]->neurons[j]->weights[k] += alpha * layers[i]->neurons[j]->inputs[k] * error;
				}
				else
				{
					layers[i]->neurons[j]->weights[k] += 
								alpha * layers[i]->neurons[j]->inputs[k] * layers[i]->neurons[j]->errorGradient;
				}
			}
			layers[i]->neurons[j]->bias += alpha * -1 * layers[i]->neurons[j]->errorGradient;
		}
	}

/*
	void UpdateWeights(List<double> outputs, List<double> desiredOutput)
	{
		double error;
		for(int i = numHidden; i >= 0; i--)
		{
			for(int j = 0; j < layers[i].numNeurons; j++)
			{
				if(i == numHidden)
				{
					error = desiredOutput[j] - outputs[j];
					layers[i].neurons[j].errorGradient = outputs[j] * (1-outputs[j]) * error;
				}
				else
				{
					layers[i].neurons[j].errorGradient = layers[i].neurons[j].output * (1-layers[i].neurons[j].output);
					double errorGradSum = 0;
					for(int p = 0; p < layers[i+1].numNeurons; p++)
					{
						errorGradSum += layers[i+1].neurons[p].errorGradient * layers[i+1].neurons[p].weights[j];
					}
					layers[i].neurons[j].errorGradient *= errorGradSum;
				}
				for(int k = 0; k < layers[i].neurons[j].numInputs; k++)
				{
					if(i == numHidden)
					{
						error = desiredOutput[j] - outputs[j];
						layers[i].neurons[j].weights[k] += alpha * layers[i].neurons[j].inputs[k] * error;
					}
					else
					{
						layers[i].neurons[j].weights[k] += alpha * layers[i].neurons[j].inputs[k] * layers[i].neurons[j].errorGradient;
					}
				}
				layers[i].neurons[j].bias += alpha * -1 * layers[i].neurons[j].errorGradient;
			}

		}

	}
*/
}

void ArtificialNN::LoadWeights(FString weightStr)
{
	if (weightStr == "") return;
	TArray<FString> weightValues;

	weightStr.ParseIntoArray(weightValues, TEXT(","));
	int w = 0;
	for (NetworkLayer *l : layers)
	{
		for(Neuron *n : l->neurons)
		{
			for (int i = 0; i < n->weights.Num(); i++)
			{
				n->weights[i] = FCString::Atod(*weightValues[w]);
				w++;
			}
			n->bias = FCString::Atod(*weightValues[w]);
			w++;
		}
	}
}

//for full list of activation function
//see en.wikipedia.org/wiki/Activation_function
double ArtificialNN::ActivationFunction(double value)
{
	return TanH(value);
}

double ArtificialNN::ActivationFunctionO(double value)
{
	return Sigmoid(value);
}

double ArtificialNN::TanH(double value)
{
	double k = (double)FMath::Exp(-2 * value);
	return 2 / (1.0f + k) - 1;
}

//also known as: logistic softstep
double ArtificialNN::Sigmoid(double value)
{
	double k = (double)FMath::Exp(-1 * value);
	return 1 / (1.0f + k);
}

double ArtificialNN::Step(double value)
{
	if (value < 0) return 0;
	else return 1;
}

double ArtificialNN::ReLu(double value)
{
	if (value > 0) return value;
	else return 0;
}

double ArtificialNN::Linear(double value)
{
	return value;
}

double ArtificialNN::LeakyReLu(double value)
{
	if (value < 0) return 0.01*value;
	else return value;
}



