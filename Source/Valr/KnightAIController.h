// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ANN/ArtificialNN.h"
#include "Knight.h"
#include "FightManager.h"

#include "KnightAIController.generated.h"
class Replay
{
public:
	TArray<double> States;	//Save this
	double Reward = 0;

	Replay(double s1, double s2, double s3, double s4, double s5, double s6, float r)
	{
		States.Add(s1);
		States.Add(s2);
		States.Add(s3);
		States.Add(s4);
		States.Add(s5);
		States.Add(s6);
		Reward = r;
	}
};
/**
 * 
 */
UCLASS()
class VALR_API AKnightAIController : public AAIController
{
	GENERATED_BODY()
public:
	TArray<Replay*> replayMemory;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		AFightManager* Manager = nullptr;

	ArtificialNN* Network = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AKnight *Fighter = nullptr;

	int BiasRand(const int arr[], int size);

	TArray<double> SoftMax(TArray<double> values);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	int attackTimer = 0;

	float attackRange[3];
	float averageRange = 0;

	float Discount = 0.99f;						//how much future states affect rewards
	float exploreRate = 100.f;					//chance of picking random action
	float maxExploreRate = 100.0f;				//max chance value
	float minExploreRate = 0.01f;				//min chance value
	float exploreDecay = 0.01f;					//chance decay amount for each update

	float Timer = 0;							//timer to keep track of balancing
	int holdMaxQIndex = 0;
	int frameDelay = 0;
	float recordedDistance = 0;

	bool bTrain = true;

	uint8 difficultyTimer = 0;
	uint8 difficultyBlockChance = 0;
};
