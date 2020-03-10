// Code by Sultan Iljasov, 2020.


#include "AIBrain.h"

AAIBrain::AAIBrain()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<double> AAIBrain::SoftMax(TArray<double> values)
{
	double max = FMath::Max<double>(values);

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

	Network = new ArtificialNN(2, 3, 6, 8, 0.8f);
	//assign the weights	

}

void AAIBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//	TArray<double> states;
//	TArray<double> qs;

//	states.Add(static_cast<double>(Fighter->Health));
//	states.Add(static_cast<double>(Manager->Player1->Health));
//	states.Add(FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()));

//	qs = Network->CalcOutput(states)
//	double maxQ = FMath::Max(qs);
//	int maxQIndex = qs.Find(maxQ);

	if (Manager->roundState != ROUND_STATE::ROUND_ONGOING) return;

	/*FString message = FString::Printf(
		TEXT("\n\n\n\
			Stats\n\
			Fails: %d\n\
			Decay Rate: %f\n\
			Last Best Balance: %f\n"),
		failCount, exploreRate, maxBalanceTime);

	GEngine->AddOnScreenDebugMessage(
		0, 0.5f, FColor::Cyan,
		message
	);*/

		dist = FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation());
		TArray<double> states;
		TArray<double> qs;

		//states.Add(static_cast<double>(Fighter->Health));
		//states.Add(static_cast<double>(Manager->Player1->Health));
		states.Add(FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()));
		states.Add(static_cast<double>(Fighter->State));
		//states.Add(static_cast<double>(Manager->Player1->State));
		//FVector toEnemyVec = (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation());
		//toEnemyVec.Normalize();
		//float forwardAngle = AFightManager::Angle(toEnemyVec, Fighter->GetActorForwardVector());
		//states.Add(forwardAngle);
		//states.Add(static_cast<double>(Fighter->bOpponentIsHit));
		//states.Add(static_cast<double>(Manager->Player1->bOpponentIsHit));

		qs = SoftMax(Network->CalcOutput(states));
		double maxQ = FMath::Max(qs);
		int maxQIndex = qs.Find(maxQ);
			exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);

		if (FMath::RandRange(0, 100) < exploreRate)
			maxQIndex = FMath::RandRange(0, qs.Num()-1);
		UE_LOG(LogTemp, Warning, TEXT("MaxQValue: %f     Action: %i     exploreRate: %f"), maxQ, maxQIndex, exploreRate)

		switch (maxQIndex)
		{
			//case 0: Fighter->ReleasedBlock(); Fighter->AxisW(1); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(0); break;
			//case 1: Fighter->ReleasedBlock(); Fighter->AxisW(1); Fighter->AxisA(1); Fighter->AxisS(0); Fighter->AxisD(0); break;
			case 0: Fighter->ReleasedBlock(); Fighter->AxisW(0); Fighter->AxisA(1); Fighter->AxisS(0); Fighter->AxisD(0); break;
			//case 3: Fighter->ReleasedBlock(); Fighter->AxisW(0); Fighter->AxisA(1); Fighter->AxisS(1); Fighter->AxisD(0); break;
			//case 4: Fighter->ReleasedBlock(); Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(1); Fighter->AxisD(0); break;
			//case 5: Fighter->ReleasedBlock(); Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(1); Fighter->AxisD(1); break;
			case 1: Fighter->ReleasedBlock(); Fighter->AxisW(0); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(1); break;
			//case 7: Fighter->ReleasedBlock(); Fighter->AxisW(1); Fighter->AxisA(0); Fighter->AxisS(0); Fighter->AxisD(1); break;

			case 2: Fighter->ReleasedBlock(); Fighter->PressedLight(); break;
			/*case 9: Fighter->ReleasedBlock(); Fighter->PressedMedium(); break;
			case 10: Fighter->ReleasedBlock(); Fighter->PressedHeavy(); break;
			case 11: Fighter->ReleasedBlock(); Fighter->PressedSpecial(); break;*/

			//case 8: Fighter->PressedBlock(); break;
			default: break;
		}
		if (Manager->Player1->bOpponentIsHit && !Failed) Failed = true;
		if (Failed && Manager->Player1->currentFrameOfAttack == 0) Failed = false;

		currentReward = 10.0f;
		if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < dist)
		{
			currentReward += 100.f;
			UE_LOG(LogTemp, Warning, TEXT("AI is getting close to enemy"))
		}
		else
		{
			currentReward += -10.0f;
			UE_LOG(LogTemp, Warning, TEXT("AI is fleeing!"))
		}

		if (Fighter->State == STATE::ATTACKING && FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 150)
		{
			currentReward += 10.0f;
			UE_LOG(LogTemp, Warning, TEXT("AI isn't hit"))
		}
		else if (Fighter->State == STATE::ATTACKING && FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > 150)
		{
			currentReward += -50.f;
			UE_LOG(LogTemp, Warning, TEXT("AI was hit"))
		}

		UE_LOG(LogTemp, Warning, TEXT("dist : %f"), FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()))
		/*if (Fighter->Health > Manager->Player1->Health)
		{
			currentReward += 50.f;
			UE_LOG(LogTemp, Warning, TEXT("AI has more health than enemy"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AI has less health than enemy")) 
			currentReward -= 10.f;
		}*/

		/*if (forwardAngle < 90)
		{
			currentReward += 0.55f;
			UE_LOG(LogTemp, Warning, TEXT("AI is facing the correct direction")) 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AI is facing the wrong direction"))
			currentReward -= 0.25;
		}*/

		UE_LOG(LogTemp, Warning, TEXT("Reward: %f"), currentReward);
		
		Replay* lastMemory = new Replay(
			FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()),
			static_cast<double>(Fighter->State),
			currentReward);

		if (replayMemory.Num() > maxCapacity)
		{
			replayMemory.RemoveAt(0);
		}

		replayMemory.Push(lastMemory);

		if (currentReward < 0)
		{
			for (int i = replayMemory.Num() - 1; i >= 0; i--)
			{
				TArray<double> t_outputsOld;
				TArray<double> t_outputsNew;
				t_outputsOld = SoftMax(Network->CalcOutput(replayMemory[i]->States));

				double maxQOld = FMath::Max(t_outputsOld);
				int action = t_outputsOld.Find(maxQOld);

				double feedback;
				if (i == replayMemory.Num() - 1 || replayMemory[i]->Reward < 0)
				{
					feedback = replayMemory[i]->Reward;
				}
				else
				{
					t_outputsNew = SoftMax(Network->CalcOutput(replayMemory[i + 1]->States));
					maxQ = FMath::Max(t_outputsNew);
					feedback = (replayMemory[i]->Reward + Discount * maxQ);
				}

				t_outputsOld[action] = feedback;
				Network->Train(replayMemory[i]->States, t_outputsOld);
			}
			Failed = false;
			replayMemory.Empty();
			UE_LOG(LogTemp,Warning,TEXT("Memory Cleared"))
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
