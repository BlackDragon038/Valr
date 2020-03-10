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
	TArray<double> States;	//Save this
	double Reward;

	Replay(double s1, double s2, float r)
	{
		States.Add(s1);
		States.Add(s2);
		//States.Add(s3);
		//States.Add(s4);
		//States.Add(s5);
		//States.Add(s6);
		//States.Add(s7);
		//States.Add(s8);
		Reward = r;
	}
};

UCLASS()
class VALR_API AAIBrain : public AAIController
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
		AFighterPawn* Fighter = nullptr;

	ArtificialNN* Network = nullptr;

	double currentReward = 0.0f;						//reward to associate with actions
	TArray<Replay*> replayMemory;					//memory - list of past actions and rewards
	int maxCapacity = 100000;						//memory capacity

	float Discount = 0.99f;						//how much future states affect rewards
	float exploreRate = 100.0f;					//chance of picking random action
	float maxExploreRate = 100.0f;					//max chance value
	float minExploreRate = 0.01f;					//min chance value
	float exploreDecay = 0.01f;					//chance decay amount for each update
	bool Failed = false;							//to track when it fails and punish it.

	int failCount = 0;						//amount of times the character was hit
	float Timer = 0;						//timer to keep track of balancing
	float maxBalanceTime = 0;					//record time ball is kept balanced	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int actionChangeRate = 200;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		AFightManager* Manager = nullptr;

	uint8 n = 0;
	uint8 t = 0;

	float dist = 0.f;

public:
	AAIBrain();

	TArray<double> SoftMax(TArray<double> values);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

};