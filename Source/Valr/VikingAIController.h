// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ANN/ArtificialNN.h"
#include "Viking.h"
#include "FightManager.h"

#include "VikingAIController.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API AVikingAIController : public AAIController
{
	GENERATED_BODY()
public:
		TArray<ArtificialNN::Replay*> replayMemory;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		AFightManager* Manager = nullptr;

	ArtificialNN* Network = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AViking *Fighter = nullptr;

	int BiasRand(const int arr[], int size);

	TArray<double> SoftMax(TArray<double> values);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	int attackTimer = 0;

	float attackRange[3];
	float averageRange = 0;

	float Discount = 0.99f;						//how much future states affect rewards
	float exploreRate = 15.f;					//chance of picking random action
	float maxExploreRate = 100.0f;				//max chance value
	float minExploreRate = 15.0f;				//min chance value
	float exploreDecay = 0.005f;					//chance decay amount for each update

	int Timer = 0;							//timer to keep track of balancing
	int holdMaxQIndex = 0;
	int frameDelay = 0;
	float recordedDistance = 0;

	bool bTrain = true;

	uint8 difficultyTimer = 0;
	uint8 difficultyBlockChance = 0;
};
