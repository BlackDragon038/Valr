// Code by Sultan Iljasov, 2020.


#include "AIBrain.h"
#include <ctime>
#include "Crusader.h"

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

int BiasRand(const int arr[], int size)
{
	int random = rand() % size;
	return arr[random];
}
void AAIBrain::BeginPlay()
{
	Super::BeginPlay();

	srand(unsigned(time(0)));
	FMath::SRandInit(unsigned(time(0)));
	Fighter = Cast<AFighterPawn>(GetOwner());
	replayMemory.Reserve(maxCapacity);
	Network = new ArtificialNN(5, 13, 3, 13, 0.5f);
	//assign the weights
}

void AAIBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	TArray<double> states;	//Prepare the arrays.
	//	TArray<double> qs;

	//	Set up the same states.

	//	qs = Network->CalcOutput(states)	//Find the correct output.
	//	double maxQ = FMath::Max(qs);
	//	int maxQIndex = qs.Find(maxQ);

	if (Manager->roundState != ROUND_STATE::ROUND_ONGOING || Fighter->currentFrameOfAttack > 0) return;

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

	if (record > 0)
	{
		record = 0;
		enemyHealth = Manager->Player1->Health;
	} else record++;

	TArray<double> states;
	TArray<double> qs;

	states.Add(static_cast<double>(Fighter->Health));
	states.Add(static_cast<double>(Fighter->Stamina));
	states.Add(FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()));
	states.Add(static_cast<double>(Fighter->State));
	states.Add(static_cast<double>(Fighter->attackType));

	qs = SoftMax(Network->CalcOutput(states));
	double maxQ = FMath::Max(qs);
	int maxQIndex = qs.Find(maxQ);
	exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);
	if (Fighter->State != STATE::MOVING) attackTimer = 0;
	int movementArr[] = { 0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4,4,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7 };
	int attackArr[] = { 0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,3,3,4,4,4,4,4,4,4,4,4,4,5,5,6,6,6,6,6,6,6,6,6,6,7,7,8,8,8,8,9,9,9,10,10,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12};
	if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > FMath::RandRange(160,180))
	{
		if (FMath::RandRange(0, 100) < exploreRate && Timer > 10)
		{
			maxQIndex = BiasRand(movementArr, 71);
			holdMaxQIndex = maxQIndex;
		}
		else if (FMath::RandRange(0, 100) < exploreRate && Timer < 10) 
			maxQIndex = holdMaxQIndex;
	}
	else
	{
		if (FMath::RandRange(0, 100) < exploreRate && Timer > 10)
		{
			maxQIndex = BiasRand(attackArr, 87);
			holdMaxQIndex = maxQIndex;
		}
		else if (FMath::RandRange(0, 100) < exploreRate && Timer < 10)
			maxQIndex = holdMaxQIndex;
	}
	if (Timer > 10) Timer = 0;
	else Timer++;

	if (maxQIndex != 11 && Fighter->attackType == ATTACK_TYPE::SPECIAL && Fighter == static_cast<ACrusader*>(Fighter)) { static_cast<ACrusader*>(Fighter)->ReleasedSpecial(); UE_LOG(LogTemp, Error, TEXT("RELEASED SPECIAL")) }
	if (maxQIndex != 12 && Fighter->State == STATE::BLOCKING) Fighter->AxisBlock(0);
	//UE_LOG(LogTemp, Warning, TEXT("Weights: %s"), *Network->PrintWeights());
	UE_LOG(LogTemp, Warning, TEXT("MaxQValue: %f	Action: %i		exploreRate: %f"), maxQ, maxQIndex, exploreRate)
	UE_LOG(LogTemp, Warning, TEXT("Input %i"), Fighter->InputID);
	
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
	default:
		UE_LOG(LogTemp, Warning, TEXT("%i ... DEFAULT...DEFAULT...DEFAULT..."), maxQIndex)
			break;
	}

	currentReward = 0.0f;
	if (Fighter->State == STATE::IDLE)
	{
		currentReward = -1.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is standing there DOING NOTHING!"), maxQIndex)
	}
	else if (Fighter->State == STATE::ATTACKING)
	{
		int rangeAverage = 0;
		int divider = 0;
		float attackRange;
		for (auto &a : Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts)
		{
			if (a.maxDist > 0) { rangeAverage += a.maxDist; divider++; }
		}
		attackRange = static_cast<float>(rangeAverage) / static_cast<float>(divider);
		if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > attackRange * 1.05f)
		{
			currentReward = -1.0f;
		}
		else
		{
			if (Fighter->attackType != ATTACK_TYPE::SPECIAL)
			currentReward = 1.0f;
			else currentReward = -1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is attacking for REAL!"), maxQIndex)
		}
	}
	else if (Fighter->State == STATE::MOVING)
	{
		if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > 175 && Fighter->Health >= 63)
		{
			currentReward = -1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is fleeing away!... distance: %f"), maxQIndex, currentDistance)
		}
		else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 175 && Fighter->Health < 63)
		{
			currentReward = -1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is too close to player!... distance : %f"), maxQIndex, currentDistance)
		}
		else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 75)
		{
			currentReward = -0.5f;
		}
		else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 175 && Fighter->Health >= 63)
		{
			if (attackTimer > 120)
			{
				currentReward = -1.0f;
			}
			else
			{
				currentReward = 0.1f;
				attackTimer++;
			}

			//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is battling forward!"), maxQIndex)
		}
	}
	else if (Fighter->State == STATE::STUNNED)
	{
		currentReward = -1.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is stunned!"), maxQIndex)
	}
	else if (Fighter->State == STATE::BLOCKING)
	{
		if (Manager->Player1->State == STATE::STUNNED)
		{
			currentReward = 1.0f;
		}
		else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < Fighter->BlockData.maxDist)
		{
			currentReward = 1.0f;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%i ... NOTHING HAPPENED ... %d"), maxQIndex, Fighter->State)
	}

	UE_LOG(LogTemp, Warning, TEXT("Reward: %f	State: %i"), currentReward, Fighter->State);

	Replay* lastMemory = new Replay(
		static_cast<double>(Fighter->Health),
		static_cast<double>(Fighter->Stamina),
		FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()),
		static_cast<double>(Fighter->State),
		static_cast<double>(Fighter->attackType),
		currentReward);

	if (replayMemory.Num() > maxCapacity)
	{
		replayMemory.RemoveAt(0);
	}

	replayMemory.Push(lastMemory);

	if (currentReward <= 0)
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
		exploreRate += (1 / exploreRate) * 0.5f;
		//UE_LOG(LogTemp, Warning, TEXT("Memory Cleared"))
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