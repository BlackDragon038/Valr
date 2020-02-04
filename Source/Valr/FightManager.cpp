// Fill out your copyright notice in the Description page of Project Settings.


#include "FightManager.h"
#include "DrawDebugHelpers.h"
#include <cmath>
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFightManager::AFightManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFightManager::BeginPlay()
{
	Super::BeginPlay();
	if (!Player1 || !Player2 || !Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("FighterManager: The pointers aren't set up in the Blueprint!"));
		return;
	}
	GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera);
	AddTickPrerequisiteActor(Player1);
	AddTickPrerequisiteActor(Player2);
}

float AFightManager::Angle(FVector a, FVector b)
{
	return FMath::RadiansToDegrees(std::acos(FVector::DotProduct(a, b)));
}

// Called every frame
void AFightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	roundTimer -= 1;
	FVector toPlayer1 = Player1->GetActorLocation() - Player2->GetActorLocation();
	FVector toPlayer2 = Player2->GetActorLocation() - Player1->GetActorLocation();
	if (toPlayer1.Size() < 50)
	{
		toPlayer1.Normalize();
		toPlayer2.Normalize();
		Player1->SetActorLocation(Player1->GetActorLocation() + toPlayer1 * Player1->MovementSpeed);
		Player2->SetActorLocation(Player2->GetActorLocation() + toPlayer2 * Player2->MovementSpeed);
	}
	else
	{
		toPlayer1.Normalize();
		toPlayer2.Normalize();
	}
	Camera->SetActorLocation(FVector((Player1->GetActorLocation().X + Player2->GetActorLocation().X) * 0.5f, (Player1->GetActorLocation().Y + Player2->GetActorLocation().Y) * 0.5f, Player1->GetActorLocation().Z + Camera->Height));
	Camera->SpringArm->TargetArmLength = FVector::Distance(Player1->GetActorLocation(), Player2->GetActorLocation());
	if (Camera->SpringArm->TargetArmLength < Camera->closestDistance) Camera->SpringArm->TargetArmLength = Camera->closestDistance;
	/*if (Player2->State != AFighterPawn::STATE::Stunned)
	{
		if (t == 0)
		{
			a = FMath::RandRange(0, 9);
			t = 100;
		}
		else
		{
			t--;
			switch (n)
			{
				case 0: Player2->PressedW(1); Player2->PressedA(0); Player2->PressedS(0); Player2->PressedD(0); break;
				case 1: Player2->PressedW(0); Player2->PressedA(1); Player2->PressedS(0); Player2->PressedD(0); break;
				case 2: Player2->PressedW(0); Player2->PressedA(0); Player2->PressedS(1); Player2->PressedD(0); break;
				case 3: Player2->PressedW(0); Player2->PressedA(0); Player2->PressedS(0); Player2->PressedD(1); break;
				case 4: Player2->PressedLight(); break;
				case 5: Player2->PressedMedium(); break;
				case 6: Player2->PressedHeavy(); break;
				case 7: Player2->PressedSpecial(); break;
				case 8: Player2->PressedBlock(); break;
				case 9: Player2->ReleasedBlock(); break;
			}
		}
	}*/

	FVector MiddleVector = Player2->GetActorLocation() - Player1->GetActorLocation();
	FVector PerpendicularVector = { MiddleVector.Y,-MiddleVector.X,MiddleVector.Z };
	PerpendicularVector.Normalize();
	Camera->SetActorRotation(FRotator(PerpendicularVector.Rotation().Roll, PerpendicularVector.Rotation().Yaw, PerpendicularVector.Rotation().Pitch));

	if (Player1->State == AFighterPawn::STATE::Moving || Player1->State == AFighterPawn::STATE::Stepping)
	{
		Player1->SetActorRotation(FMath::Lerp(Player1->GetActorRotation(), toPlayer2.Rotation(), 0.35f));
	}
	else if (Player1->State == AFighterPawn::STATE::Attacking)
	{
		if (Player1->currentFrameOfAttack >= Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].PSum)
		{
			if (Player1->currentPartsIndex < Player1->Attacks[Player1->attackType].Parts.Num() - 1)
				Player1->currentPartsIndex++;
			else
			{
				Player1->State = AFighterPawn::STATE::Idle;
				Player1->InputID = AFighterPawn::INPUT::IDLE;
				Player1->currentFrameOfAttack = 0;
				Player1->currentPartsIndex = 0;
				Player1->attackType = AFighterPawn::ATTACK_TYPE::NONE;
				bPlayer2IsHit = false;
				return;
			}
		}
		if (Angle(Player1->GetActorRightVector(), toPlayer2) > Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minAngle &&
			Angle(Player1->GetActorRightVector(), toPlayer2) < Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxAngle &&
			(Player2->GetActorLocation() - Player1->GetActorLocation()).Size() > Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minDist &&
			(Player2->GetActorLocation() - Player1->GetActorLocation()).Size() < Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxDist &&
			Angle(Player1->GetActorForwardVector(), toPlayer2) < 90)
		{
			if (Player2->State == AFighterPawn::STATE::Blocking && Angle(Player2->GetActorForwardVector(), toPlayer1) < Player2->BlockData.Angle &&
				(Player1->GetActorLocation() - Player2->GetActorLocation()).Size() > Player2->BlockData.minDist &&
				(Player1->GetActorLocation() - Player2->GetActorLocation()).Size() < Player2->BlockData.maxDist)
			{
				Player1->State = AFighterPawn::STATE::Stunned;
				Player1->currentFrameOfAttack = Player2->BlockData.blockStunRate;
				Player1->stunPush = Player2->BlockData.blockPushPower;
			}
			else
			{
				if (!bPlayer2IsHit)
				{
					Player2->Health -= Player1->Attacks[Player1->attackType].Damage;
					Player2->State = AFighterPawn::STATE::Stunned;
					Player2->currentFrameOfAttack = Player1->Attacks[Player1->attackType].StunRate;
					bPlayer2IsHit = true;
					Player2->stunPush = Player1->Attacks[Player1->attackType].StunPushPower;
				}
				Player1->currentFrameOfAttack++;
			}
		}
		else
		{
			Player1->currentFrameOfAttack++;
		}
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Min Angle: %f"), Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minAngle));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Max Angle: %f"), Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxAngle));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Min Distance: %f"), Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minDist));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Max Distance: %f"), Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxDist));
	}
	else if (Player1->State == AFighterPawn::STATE::Stunned)
	{
		if (Player1->currentFrameOfAttack > 0)
		{
			Player1->currentFrameOfAttack--;
			Player1->SetActorLocation(Player1->GetActorLocation() + (toPlayer1 * (Player1->currentFrameOfAttack * ((float)Player1->stunPush/100.f))));
		}
		else
		{
			Player1->State = AFighterPawn::STATE::Idle;
			Player1->InputID = AFighterPawn::INPUT::IDLE;
			Player1->currentPartsIndex = 0;
			Player1->stunPush = 1;
			Player1->attackType = AFighterPawn::ATTACK_TYPE::NONE;
		}
	}

	///****************************************PLAYER 2****************************************
	if (Player2->State == AFighterPawn::STATE::Moving || Player2->State == AFighterPawn::STATE::Stepping)
	{
		Player2->SetActorRotation(FMath::Lerp(Player2->GetActorRotation(), toPlayer1.Rotation(), 0.35f));
	}
	else if (Player2->State == AFighterPawn::STATE::Attacking)
	{
		if (Player2->currentFrameOfAttack >= Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].PSum)
		{
			if (Player2->currentPartsIndex < Player2->Attacks[Player2->attackType].Parts.Num() - 1)
				Player2->currentPartsIndex++;
			else
			{

				Player2->State = AFighterPawn::STATE::Idle;
				Player2->InputID = AFighterPawn::INPUT::IDLE;
				Player2->currentFrameOfAttack = 0;
				Player2->currentPartsIndex = 0;
				Player2->attackType = AFighterPawn::ATTACK_TYPE::NONE;
				bPlayer1IsHit = false;
				return;
			}
		}
		if (Angle(Player2->GetActorRightVector(), toPlayer1) > Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].minAngle &&
			Angle(Player2->GetActorRightVector(), toPlayer1) < Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].maxAngle &&
			(Player1->GetActorLocation() - Player2->GetActorLocation()).Size() > Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].minDist &&
			(Player1->GetActorLocation() - Player2->GetActorLocation()).Size() < Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].maxDist &&
			Angle(Player2->GetActorForwardVector(), toPlayer1) < 90)
		{
			if (Player1->State == AFighterPawn::STATE::Blocking && Angle(Player1->GetActorForwardVector(), toPlayer2) < Player1->BlockData.Angle &&
				(Player2->GetActorLocation() - Player1->GetActorLocation()).Size() > Player1->BlockData.minDist &&
				(Player2->GetActorLocation() - Player1->GetActorLocation()).Size() < Player1->BlockData.maxDist)
			{
				Player2->State = AFighterPawn::STATE::Stunned;
				Player2->currentFrameOfAttack = Player1->BlockData.blockStunRate;
				Player2->stunPush = Player1->BlockData.blockPushPower;
			}
			else
			{
				if (!bPlayer1IsHit)
				{
					Player1->Health -= Player2->Attacks[Player2->attackType].Damage;
					Player1->State = AFighterPawn::STATE::Stunned;
					Player1->currentFrameOfAttack = Player2->Attacks[Player2->attackType].StunRate;
					bPlayer1IsHit = true;
					Player1->stunPush = Player2->Attacks[Player2->attackType].StunPushPower;
				}
				Player2->currentFrameOfAttack++;
			}
		}
		else
		{
			Player2->currentFrameOfAttack++;
		}
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Min Angle: %f"), Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].minAngle));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Max Angle: %f"), Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].maxAngle));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Min Distance: %f"), Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].minDist));
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Attack Max Distance: %f"), Player2->Attacks[Player2->attackType].Parts[Player2->currentPartsIndex].maxDist));
	}
	else if (Player2->State == AFighterPawn::STATE::Stunned)
	{
		if (Player2->currentFrameOfAttack > 0)
		{
			Player2->currentFrameOfAttack--;
			Player2->SetActorLocation(Player2->GetActorLocation() + (toPlayer2 * (Player2->currentFrameOfAttack * ((float)Player2->stunPush/100.f))));
		}
		else
		{
			Player2->State = AFighterPawn::STATE::Idle;
			Player2->InputID = AFighterPawn::INPUT::IDLE;
			Player2->currentPartsIndex = 0;
			Player2->stunPush = 1;
			Player2->attackType = AFighterPawn::ATTACK_TYPE::NONE;
			/*During an attack, the test for detection will be done during attack frames.
			If the attack does hit an enemy, bPlayerXIsHit is set to true to avoid the attack be applied many times.
			When the attack hits, the enemy is also stunned, meaning that this code will run.
			Since we don't know how the stun was caused, we have to check and see that it wasn't caused by a normal attack.
			If we don't do this, we mistakenly will set bPlayerXIsHit and allow the attack to be applied again.*/
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 Angle: %f  -  Player 2 to Player 1 Distance: %f"), Angle(Player2->GetActorRightVector(), toPlayer1), (Player1->GetActorLocation() - Player2->GetActorLocation()).Size()));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("IsPlayerHit1? %i"), bPlayer1IsHit));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 State: %i"), Player2->State));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 InputID: %i"), Player2->InputID));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 CurrentFrameOfAttack: %i"), Player2->currentFrameOfAttack));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 CurrentPartsIndex: %i"), Player2->currentPartsIndex));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 AttackType: %i"), Player2->attackType));

	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::White, FString::Printf(TEXT("FrameTime: %f----------------------FrameRate: %i"), DeltaTime, 1000 / DeltaTime));

	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("Player 1 Angle: %f  -  Player 1 to Player 2 Distance: %f"), Angle(Player1->GetActorRightVector(), toPlayer2), (Player2->GetActorLocation() - Player1->GetActorLocation()).Size()));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("IsPlayerHit2? %i"), bPlayer2IsHit));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, FString::Printf(TEXT("Player 1 State: %i"), Player1->State));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, FString::Printf(TEXT("Player 1 InputID: %i"), Player1->InputID));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("Player 1 CurrentFrameOfAttack: %i"), Player1->currentFrameOfAttack));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("Player 1 CurrentPartsIndex: %i"), Player1->currentPartsIndex));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, FString::Printf(TEXT("Player 1 AttackType: %i"), Player1->attackType));

	DrawDebugLine
	(
		GetWorld(),
		Player1->GetActorLocation(),
		Player2->GetActorLocation(),
		FColor::Red,
		false,
		1,
		1,
		1);

	DrawDebugLine
	(
		GetWorld(),
		Player1->GetActorLocation(),
		Player1->GetActorLocation() + (Player1->GetActorRightVector() * 100),
		FColor::Red,
		false,
		1,
		1,
		1);
}

// Called to bind functionality to input
void AFightManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Player1_W", Player1, &AFighterPawn::AxisW);
	PlayerInputComponent->BindAxis("Player1_A", Player1, &AFighterPawn::AxisA);
	PlayerInputComponent->BindAxis("Player1_S", Player1, &AFighterPawn::AxisS);
	PlayerInputComponent->BindAxis("Player1_D", Player1, &AFighterPawn::AxisD);
	PlayerInputComponent->BindAction("Player1_Light", IE_Pressed, Player1, &AFighterPawn::PressedLight);
	PlayerInputComponent->BindAction("Player1_Medium", IE_Pressed, Player1, &AFighterPawn::PressedMedium);
	PlayerInputComponent->BindAction("Player1_Heavy", IE_Pressed, Player1, &AFighterPawn::PressedHeavy);
	PlayerInputComponent->BindAction("Player1_Special", IE_Pressed, Player1, &AFighterPawn::PressedSpecial);
	PlayerInputComponent->BindAxis("Player1_Block", Player1, &AFighterPawn::AxisBlock);

	PlayerInputComponent->BindAxis("Player2_W", Player2, &AFighterPawn::AxisS);
	PlayerInputComponent->BindAxis("Player2_A", Player2, &AFighterPawn::AxisD);
	PlayerInputComponent->BindAxis("Player2_S", Player2, &AFighterPawn::AxisW);
	PlayerInputComponent->BindAxis("Player2_D", Player2, &AFighterPawn::AxisA);
	PlayerInputComponent->BindAction("Player2_Light", IE_Pressed, Player2, &AFighterPawn::PressedLight);
	PlayerInputComponent->BindAction("Player2_Medium", IE_Pressed, Player2, &AFighterPawn::PressedMedium);
	PlayerInputComponent->BindAction("Player2_Heavy", IE_Pressed, Player2, &AFighterPawn::PressedHeavy);
	PlayerInputComponent->BindAction("Player2_Special", IE_Pressed, Player2, &AFighterPawn::PressedSpecial);
	PlayerInputComponent->BindAxis("Player2_Block", Player2, &AFighterPawn::AxisBlock);

}

