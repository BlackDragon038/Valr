// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FightManager.h"
#include "ANN/ArtificialNN.h"

#include "AIBrain.generated.h"

class Replay
{
public:
	TArray<double> Variables;
	double Reward;

	Replay(double y, double v, double x, double h, double s1, double s2, float r)
	{
		Variables.Add(y);
		Variables.Add(v);
		Variables.Add(x);
		Variables.Add(h);
		Variables.Add(s1);
		Variables.Add(s2);
		Reward = r;
	}
};

UCLASS()
class VALR_API AAIBrain : public AAIController
{
	GENERATED_BODY()
public:
	AFighterPawn *Fighter = nullptr;

	ArtificialNN* Network = nullptr;

	double currentReward = 0.0f;						//reward to associate with actions
	TArray<Replay*> replayMemory;					//memory - list of past actions and rewards
	int maxCapacity = 100;						//memory capacity

	float Discount = 1.00f;						//how much future states affect rewards
	float exploreRate = 500.0f;					//chance of picking random action
	float maxExploreRate = 150.0f;					//max chance value
	float minExploreRate = 0.01f;					//min chance value
	float exploreDecay = 0.0001f;					//chance decay amount for each update
	bool Failed = false;							//to track when it fails and punish it.

	int failCount = 0;						//amount of times the character was hit
	float Timer = 0;						//timer to keep track of balancing
	float maxBalanceTime = 0;					//record time ball is kept balanced	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float actionChangeRate = 0.01f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		AFightManager *Manager = nullptr;

	uint8 n = 0;
	uint8 t = 0;

public:
	AAIBrain();

	TArray<int> SoftMax(TArray<int> values);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

};
