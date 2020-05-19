// Code by Sultan Iljasov, 2020.


#include "SoldierAIController.h"
#include <ctime>
#include <string>

int ASoldierAIController::BiasRand(const int arr[], int size)
{
	int random = FMath::RandRange(0, size);
	return arr[random];
}

TArray<double> ASoldierAIController::SoftMax(TArray<double> values)
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

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();
	Fighter = Cast<ASoldier>(GetOwner());
	Network = new ArtificialNN(7, 13, 5, 15, 1);
	FMath::SRandInit(unsigned(time(0)));

	for (int i{ 0 }; i < 3; i++)
	{
		int rangeAverage = 0;
		int divider = 0;
		for (auto &a : Fighter->Attacks[i].Parts)
		{
			if (a.maxDist > 0) { rangeAverage += a.maxDist; divider++; }
		}
		attackRange[i] = static_cast<float>(rangeAverage) / static_cast<float>(divider);
	}
	averageRange = (attackRange[0] + attackRange[1] + attackRange[2]) / 3.f;
	std::string path;
	switch (Manager->Instance->Difficulty)
	{
	case DIFFICULTY::EASY: difficultyTimer = 35; difficultyBlockChance = 20; path = TCHAR_TO_UTF8(*(FPaths::ProjectContentDir() + "model_Soldier_EASY.wdt")); break;
	case DIFFICULTY::NORMAL: difficultyTimer = 25; difficultyBlockChance = 35; path = TCHAR_TO_UTF8(*(FPaths::ProjectContentDir() + "model_Soldier_NORMAL.wdt")); break;
	case DIFFICULTY::HARD: difficultyTimer = 15; difficultyBlockChance = 50; path = TCHAR_TO_UTF8(*(FPaths::ProjectContentDir() + "model_Soldier_HARD.wdt")); break;
	}
	Network->LoadWeights(path.c_str());
}

void ASoldierAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Manager->roundState != ROUND_STATE::ROUND_ONGOING || Fighter->currentFrameOfAttack > 0) return;

	TArray<double> states;
	TArray<double> qs;

	if (frameDelay > 1)
	{
		frameDelay = 0;
		recordedDistance = FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation());
	}
	else frameDelay++;

	states.Add(static_cast<double>(Fighter->Health));
	states.Add(static_cast<double>(Fighter->Stamina));
	states.Add(FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()));
	states.Add(static_cast<double>(Fighter->State));
	states.Add(static_cast<double>(Fighter->attackType));
	states.Add(static_cast<double>(AFightManager::Angle(Fighter->GetActorForwardVector(), (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).GetSafeNormal())));
	states.Add(static_cast<double>(Manager->Player1->State));

	qs = SoftMax(Network->CalcOutput(states));
	double maxQ = FMath::Max(qs);
	int maxQIndex = qs.Find(maxQ);

	if (bTrain)
	{
		exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);
		int movementArr[] = { 0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,3,3,4,4,4,4,4,4,4,4,4,4,5,5,6,6,6,6,6,6,6,6,6,6,7,7 };
		int attackArr[] = { 8,9,10 };
		FVector Dir = Manager->Player1->GetActorLocation() - Fighter->GetActorLocation();
		Dir.Normalize();
		if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > averageRange * 1.15f || AFightManager::Angle(Fighter->GetActorForwardVector(), Dir) > 70)
		{
			if (FMath::RandRange(0, 100) < 30 && Fighter->specialMeter >= Fighter->Attacks[3].staminaCost)
			{
				if (Timer == 0)
				{
					maxQIndex = 11;
				}
			}
			else
			{
				if (FMath::RandRange(0, 100) < exploreRate && Timer == 0)
				{
					maxQIndex = BiasRand(movementArr, 47);
					holdMaxQIndex = maxQIndex;
					if (holdMaxQIndex != 8 && holdMaxQIndex != 9 && holdMaxQIndex != 10) Timer = difficultyTimer;
				}
				else if (Timer > 0)
				{
					maxQIndex = holdMaxQIndex;
					Timer--;
				}
			}
		}
		else
		{
			int blockChance = 0;
			if (Manager->Player1->State == STATE::ATTACKING)
				blockChance = difficultyBlockChance;
			else blockChance = difficultyBlockChance / 2.5;
			if ((FMath::RandRange(0, 100) < blockChance) || (Timer > 0 && holdMaxQIndex == 12))
			{
				if (Timer == 0)
				{
					maxQIndex = 12;
					holdMaxQIndex = 12;
					if (Manager->Player1->State == STATE::ATTACKING)
					{
						Timer = Manager->Player1->Attacks[static_cast<uint8>(Manager->Player1->attackType)].AttackTotalFrameCount;
					}
					else
					{
						switch (Manager->Instance->Difficulty)
						{
						case DIFFICULTY::EASY: Timer = 15; break;
						case DIFFICULTY::NORMAL: Timer = 20; break;
						case DIFFICULTY::HARD: Timer = 25; break;
						}
					}
				}
				if (Manager->Player1->State == STATE::STUNNED && holdMaxQIndex == 12 && Timer > Timer / 2)
				{
					Timer -= FMath::Clamp(Timer / 2, 0, 25);
				}
				else if (Timer > 0)
				{
					maxQIndex = holdMaxQIndex;
					Timer--;
				}
			}
			else
			{
				if (FMath::RandRange(0, 100) < exploreRate && Timer == 0)
				{
					int attackChance = 0;
					switch (Manager->Instance->Difficulty)
					{
					case DIFFICULTY::EASY: attackChance = 40; break;
					case DIFFICULTY::NORMAL: attackChance = 65; break;
					case DIFFICULTY::HARD: attackChance = 90; break;
					}
					if (Fighter->Stamina > Fighter->Attacks[2].staminaCost && FMath::RandRange(0, 100) < attackChance)
					{
						maxQIndex = BiasRand(attackArr, 2);
					}
					else
					{
						maxQIndex = BiasRand(movementArr, 47);
					}
					holdMaxQIndex = maxQIndex;
					if (holdMaxQIndex != 8 && holdMaxQIndex != 9 && holdMaxQIndex != 10) Timer = difficultyTimer;
				}
				else if (Timer > 0)
				{
					maxQIndex = holdMaxQIndex;
					Timer--;
				}
			}
		}
	}

	if (maxQIndex != 12 && Fighter->State == STATE::BLOCKING) Fighter->AxisBlock(0);

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
	if (frameDelay > 0)
	{
		float currentReward = 0.0f;
		if (Fighter->State == STATE::ATTACKING)
		{
			if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > attackRange[static_cast<uint8>(Fighter->attackType)] * 1.1f)
			{
				currentReward = -1.0f;
			}
			else
			{
				FVector Dir = Manager->Player1->GetActorLocation() - Fighter->GetActorLocation();
				Dir.Normalize();
				if (AFightManager::Angle(Fighter->GetActorForwardVector(), Dir) < 70)
				{
					currentReward = 1.0f;
				}
				else currentReward = -1.0f;
				//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is attacking for REAL!"), maxQIndex)
			}
		}
		else if (Fighter->State == STATE::MOVING)
		{
			if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > recordedDistance)
			{
				currentReward = -0.1f;
				//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is fleeing away!... distance: %f"), maxQIndex, currentDistance)
			}
			else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < 120)
			{
				currentReward = -0.1f;
			}
			else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < recordedDistance)
			{
				FVector Dir = Manager->Player1->GetActorLocation() - Fighter->GetActorLocation();
				Dir.Normalize();
				if (AFightManager::Angle(Fighter->GetActorForwardVector(), Dir) < 70)
				{
					currentReward = 0.05f;
				}
				else currentReward = -0.1f;
			}
		}
		else if (Fighter->State == STATE::STUNNED)
		{
			currentReward = -1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("%i ... AI is stunned!"), maxQIndex)
		}
		else if (Fighter->State == STATE::BLOCKING)
		{
			if (Manager->Player1->State == STATE::ATTACKING)
			{
				currentReward = 1.0f;
			}
			if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < Fighter->BlockData.maxDist)
			{
				currentReward = 0.25f;
			}
			else
			{
				currentReward = -1.0f;
			}
		}
		else if (Fighter->State == STATE::STEPPING)
		{
			if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > recordedDistance && Fighter->Health <= 60)
			{
				currentReward = 0.1f;
			}
			else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) > recordedDistance && Fighter->Health >= 60)
			{
				currentReward = -0.1f;
			}
			else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < recordedDistance && Fighter->Health <= 60)
			{
				currentReward = -0.1f;
			}
			else if (FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()) < recordedDistance && Fighter->Health >= 60)
			{
				currentReward = 0.1f;
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("%i ... NOTHING HAPPENED ... %d"), maxQIndex, Fighter->State)
		}

		ArtificialNN::Replay* lastMemory = new ArtificialNN::Replay(
			static_cast<double>(Fighter->Health),
			static_cast<double>(Fighter->Stamina),
			FVector::Distance(Fighter->GetActorLocation(), Manager->Player1->GetActorLocation()),
			static_cast<double>(Fighter->State),
			static_cast<double>(Fighter->attackType),
			static_cast<double>(AFightManager::Angle(Fighter->GetActorForwardVector(), (Manager->Player1->GetActorLocation() - Fighter->GetActorLocation()).GetSafeNormal())),
			static_cast<double>(Manager->Player1->State),
			currentReward);

		if (replayMemory.Num() > 5000)
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
					feedback = (replayMemory[i]->Reward + 0.99f * maxQ);
				}

				t_outputsOld[action] = feedback;
				Network->Train(replayMemory[i]->States, t_outputsOld);
			}
			replayMemory.Empty();
			if (exploreRate <= 1 && bTrain) exploreRate += 0.1;
		}
	}

	if (bTrain)
	{
		FDateTime time;
		if (exploreRate <= minExploreRate + 1 && time.GetSecond() == 0)
		{
			switch (Manager->Instance->Difficulty)
			{
			case DIFFICULTY::EASY: Network->SaveWeights("model_Soldier_EASY"); break;
			case DIFFICULTY::NORMAL: Network->SaveWeights("model_Soldier_NORMAL"); break;
			case DIFFICULTY::HARD: Network->SaveWeights("model_Soldier_HARD"); break;
			}
		}
	}
}
