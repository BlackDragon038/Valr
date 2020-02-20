// Code by Sultan Iljasov, 2020.


#include "AIBrain.h"

AAIBrain::AAIBrain()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<int> AAIBrain::SoftMax(TArray<int> values)
{
	//double max = values.Max();
	int max = FMath::Max<int>(values);

	float scale = 0.0f;
	for (int i = 0; i < values.Num(); ++i)
		scale += FMath::Exp((int)(values[i] - max));

	TArray<int> result;
	for (int i = 0; i < values.Num(); ++i)
		result.Add(FMath::Exp((int)(values[i] - max)) / scale);

	return result;
}

void AAIBrain::BeginPlay()
{
	Super::BeginPlay();

	//PrimaryActorTick.TickInterval = actionChangeRate;

	Fighter = Cast<AFighterPawn>(GetPawn());

	Network = new ArtificialNN(6, 6, 6, 12, 0.2f);
}

void AAIBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString message = FString::Printf(
		TEXT("\n\n\n\
			Stats\n\
			Total Fails: %d\n\
			Decay Rate: %f\n\
			Best Survival Duration: %f\n\
			Current Survival Duration: %f"),
		failCount, exploreRate, maxBalanceTime, Timer);

	GEngine->AddOnScreenDebugMessage(
		0, 0.5f, FColor::Cyan,
		message
	);
	if (Fighter->State != STATE::STUNNED && Fighter->State != STATE::STEPPING && Manager->roundState == ROUND_STATE::ROUND_ONGOING && Fighter->State != STATE::GETTING_UP && Fighter->State != STATE::KNOCKED_DOWN && Fighter->Health != 0)
	{
		Timer += DeltaTime;

		TArray<int> states;
		TArray<int> qs;

		states.Add(Fighter->Health);
		states.Add(Fighter->Stamina);
		states.Add((Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).Size());
		states.Add(Manager->Player1->Health);
		states.Add(static_cast<uint8>(Manager->Player1->State));
		states.Add(static_cast<uint8>(Fighter->State));

		qs = SoftMax(Network->CalcOutput(states));
		double maxQ = FMath::Max(qs);
		int maxQIndex = qs.Find(maxQ);
		exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);

		GEngine->AddOnScreenDebugMessage(
			0, 0.5f, FColor::Red,
			FString::Printf(TEXT("Index: %i"), maxQIndex)
		);

		switch (maxQIndex)
		{
			case 0: Fighter->AxisW(1); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 1: Fighter->AxisW(1); Fighter->AxisA(1); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 2: Fighter->AxisW(0); Fighter->AxisA(1); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 3: Fighter->AxisW(0); Fighter->AxisA(1); Fighter->AxisS(1); Fighter->AxisD(0); break;
			case 4: Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(1); Fighter->AxisD(0); break;
			case 5: Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(1); Fighter->AxisD(1); break;
			case 6: Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(1); break;
			case 7: Fighter->AxisW(1); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(1); break;
			case 8: Fighter->PressedLight(); break;
			case 9: Fighter->PressedMedium(); break;
			case 10: Fighter->PressedHeavy(); break;
			case 11: Fighter->PressedSpecial(); break;
			case 12: Fighter->AxisBlock(1); break;
			case 13: Fighter->AxisBlock(0); break;
		}
		if (Manager->Player1->bOpponentIsHit)
		{
			currentReward = -1.0f;
			Failed = true;
		}
		else
		{
			currentReward = 0.1f;
		}

		if (Manager->Player1->Health > Fighter->Health)
		{
			currentReward = -2.0f;
			Failed = true;
		}
		else
		{
			currentReward = 0.1f;
		}

		if (Fighter->Stamina < 50)
		{
			currentReward = -5.0f;
			Failed = true;
		}
		else
		{
			currentReward = 0.1f;
		}

		Replay* lastMemory = new Replay(
			Fighter->Health,
			Fighter->Stamina,
			(Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).Size(),
			Manager->Player1->Health,
			static_cast<uint8>(Manager->Player1->State),
			static_cast<uint8>(Fighter->State),
			currentReward);

		if (replayMemory.Num() > maxCapacity)
			replayMemory.RemoveAt(0);

		replayMemory.Add(lastMemory);

		if (Failed)
		{
			for (int i = replayMemory.Num() - 1; i >= 0; i--)
			{
				TArray<int> t_outputsOld;
				TArray<int> t_outputsNew;
				t_outputsOld = SoftMax(Network->CalcOutput(replayMemory[i]->Variables));

				double maxQOld = FMath::Max(t_outputsOld);
				int action = t_outputsOld.Find(maxQOld);

				double feedback;
				if (i == replayMemory.Num() - 1 || replayMemory[i]->Reward == -1)
				{
					feedback = replayMemory[i]->Reward;
				}
				else
				{
					t_outputsNew = SoftMax(Network->CalcOutput(replayMemory[i + 1]->Variables));
					maxQ = FMath::Max(t_outputsNew);
					feedback = (replayMemory[i]->Reward + Discount * maxQ);
				}

				t_outputsOld[action] = feedback;
				Network->Train(replayMemory[i]->Variables, t_outputsOld);
			}

			if (Timer > maxBalanceTime)
			{
				maxBalanceTime = Timer;
			}

			Timer = 0;
			replayMemory.Empty();
			failCount++;
		}
	}

	/*if (t >= actionChangeRate)
	{
		float Distance = (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).Size();
		n = (Distance > 200) ? FMath::RandRange(0, 3) : FMath::RandRange(0, 9);
		t = 0;
	}
	else
	{
		if (Fighter->State != STATE::STUNNED && Fighter->State != STATE::STEPPING && Manager->roundState == ROUND_STATE::ROUND_ONGOING)
		{
			switch (n)
			{
			case 0: Fighter->AxisW(1); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 1: Fighter->AxisW(0); Fighter->AxisA(1); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 2: Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(1); Fighter->AxisD(0); break;
			case 3: Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(1); break;
			case 4: Fighter->PressedLight(); break;
			case 5: Fighter->PressedMedium(); break;
			case 6: Fighter->PressedHeavy(); break;
			case 7: Fighter->PressedSpecial(); break;
			case 8: Fighter->AxisBlock(1); break;
			case 9: Fighter->AxisBlock(0); break;
			}
		}
		t++;
	}*/
	
}
