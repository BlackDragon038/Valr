// Code by Sultan Iljasov, 2020.


#include "AIBrain.h"

AAIBrain::AAIBrain()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<double> AAIBrain::SoftMax(TArray<double> values)
{
	double max = values.Max();
	//double max = FMath::Max<double>(values);

	float scale = 0.0f;
	for (int i = 0; i < values.Num(); ++i)
		scale += FMath::Exp((float)(values[i] - max));

	TArray<double> result;
	for (int i = 0; i < values.Num(); ++i)
		result.Add(FMath::Exp((float)(values[i] - max)) / scale);

	return result;
}

void AAIBrain::BeginPlay()
{
	Super::BeginPlay();

	//PrimaryActorTick.TickInterval = actionChangeRate;

	Fighter = Cast<AFighterPawn>(GetOwner());

	Network = new ArtificialNN(3, 14, 2, 6, 0.2f);
	//assign the weights	

}

void AAIBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//	states.Add(static_cast<double>(Fighter->Health));
//	states.Add(static_cast<double>(Manager->Player1->Health));
//	states.Add(FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()));
//	output = Network->CalcOutput(states)


	Timer += DeltaTime;

	TArray<double> states;
	TArray<double> qs;

	states.Add(static_cast<double>(Fighter->Health));
	states.Add(static_cast<double>(Manager->Player1->Health));
	states.Add(FVector::Distance(Fighter->GetActorLocation(),Manager->Player1->GetActorLocation()));


	qs = SoftMax(Network->CalcOutput(states));
	double maxQ = FMath::Max(qs);
	int maxQIndex = qs.Find(maxQ);
	exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);

	FString message = FString::Printf(
		TEXT("\n\n\n\
			Index: %i \n\
			Stats\n\
			Total Fails: %d\n\
			Decay Rate: %f\n\
			Best Survival Duration: %f\n\
			Current Survival Duration: %f"),
		maxQIndex,failCount, exploreRate, maxBalanceTime, Timer);

	GEngine->AddOnScreenDebugMessage(
		0, 0.5f, FColor::Cyan,
		message
	);


	if (Fighter->State != STATE::STUNNED && Fighter->State != STATE::STEPPING && Manager->roundState == ROUND_STATE::ROUND_ONGOING && Fighter->State != STATE::GETTING_UP && Fighter->State != STATE::KNOCKED_DOWN && Fighter->Health != 0)
	{
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
			default: break;
		}
		if (Manager->Player1->bOpponentIsHit && !Failed) Failed = true;
		if (Failed && Manager->Player1->currentFrameOfAttack == 0) Failed = false;
		if (!Failed)
		{
			currentReward = 0.1f;
		}
		else currentReward -= 1.0f;


		if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 175)
		{
			currentReward += 0.25f;
		}
		else
			currentReward -= 0.75f;
		if (Fighter->Health > Manager->Player1->Health)
			currentReward += 0.25f;
		else currentReward -= 0.1f;

		
		Replay* lastMemory = new Replay(
			static_cast<double>(Fighter->Health),
			static_cast<double>(Manager->Player1->Health),
			FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()),
			currentReward);

		if (replayMemory.Num() > maxCapacity)
			replayMemory.RemoveAt(0);

		replayMemory.Add(lastMemory);

		if (Manager->Player1->bOpponentIsHit && Fighter->Health < Manager->Player1->Health)
		{
			for (int i = replayMemory.Num() - 1; i >= 0; i--)
			{
				TArray<double> t_outputsOld;
				TArray<double> t_outputsNew;
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
			Failed = false;
		}
	}

	/*if (t >= actionChangeRate)
	{
		float Distance = (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).Size();
		n = (Distance > 200 || Manager->Angle(Fighter->GetActorForwardVector(), (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation())) > 45) ? FMath::RandRange(0, 3) : FMath::RandRange(4, 7);
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
			}
		}
		t++;
	}*/
	
}
