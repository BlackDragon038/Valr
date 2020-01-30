// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include <cmath>
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFighterManager::AFighterManager() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFighterManager::BeginPlay() 
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

float AFighterManager::Angle(FVector a, FVector b)
{
	return FMath::RadiansToDegrees(std::acos(FVector::DotProduct(a, b) / a.Size() * b.Size()));
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	roundTimer -= 1;
	FVector toPlayer1 = Player1->GetActorLocation() - Player2->GetActorLocation();
	toPlayer1.Normalize();
	FVector toPlayer2 = Player2->GetActorLocation() - Player1->GetActorLocation();
	toPlayer2.Normalize();

	Camera->SetActorLocation(FVector((Player1->GetActorLocation().X + Player2->GetActorLocation().X) * 0.5f, (Player1->GetActorLocation().Y + Player2->GetActorLocation().Y) * 0.5f, Player1->GetActorLocation().Z + Camera->Height));
	Camera->SpringArm->TargetArmLength = FVector::Distance(Player1->GetActorLocation(), Player2->GetActorLocation());
	if (Camera->SpringArm->TargetArmLength < Camera->closestDistance) Camera->SpringArm->TargetArmLength = Camera->closestDistance;

	if (Player2->State != AFighterPawn::STATE::Stunned) Player2->InputID = AFighterPawn::INPUT::Block;
	if (Player2->State != AFighterPawn::STATE::Stunned) Player2->State = AFighterPawn::STATE::Blocking;

	FVector MiddleVector = Player2->GetActorLocation() - Player1->GetActorLocation();
	FVector PerpendicularVector = { MiddleVector.Y,-MiddleVector.X,MiddleVector.Z };
	PerpendicularVector.Normalize();
	Camera->SetActorRotation(PerpendicularVector.Rotation());

	if (Player1->State == AFighterPawn::STATE::Moving)
	{
		Player1->SetActorRotation(FMath::Lerp(Player1->GetActorRotation(),toPlayer2.Rotation(),0.5f));
		if (Angle(Player1->GetActorForwardVector(), toPlayer2) < 90 && 
			(Player1->InputID == AFighterPawn::INPUT::LEFT_DOWN || Player1->InputID == AFighterPawn::INPUT::LEFT || Player1->InputID == AFighterPawn::INPUT::UP_LEFT))
		{
			Player1->Stamina -= 2;
		}
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
				if (!bPlayer1IsHit)
				{
					Player1->State = AFighterPawn::STATE::Stunned;
					Player1->currentFrameOfAttack = Player2->blockStunRate;
					bPlayer1IsHit = true;
				}
			}
			else
			{
				if (!bPlayer2IsHit)
				{
					Player2->Health -= Player1->Attacks[Player1->attackType].Damage;
					Player2->State = AFighterPawn::STATE::Stunned;
					Player2->currentFrameOfAttack = Player1->Attacks[Player1->attackType].StunRate;
					bPlayer2IsHit = true;
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
			Player1->SetActorLocation(Player1->GetActorLocation() + (toPlayer1 * (Player1->currentFrameOfAttack / 5)));
		}
		else
		{
			Player1->State = AFighterPawn::STATE::Idle;
			Player1->InputID = AFighterPawn::INPUT::IDLE;
			Player1->currentPartsIndex = 0;
			Player1->attackType = AFighterPawn::ATTACK_TYPE::NONE;
			bPlayer1IsHit = false;
		}
	}

	///****************************************PLAYER 2****************************************
	if (Player2->State == AFighterPawn::STATE::Moving)
	{
		Player2->SetActorRotation(toPlayer1.Rotation());
		if (Angle(Player2->GetActorForwardVector(), toPlayer1) < 90 &&
			(Player2->InputID == AFighterPawn::INPUT::DOWN_RIGHT || Player2->InputID == AFighterPawn::INPUT::RIGHT || Player2->InputID == AFighterPawn::INPUT::RIGHT_UP))
		{
			Player2->Stamina -= 2;
		}
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
				if (!bPlayer2IsHit)
				{
					Player2->State = AFighterPawn::STATE::Stunned;
					Player2->currentFrameOfAttack = Player1->blockStunRate;
					bPlayer2IsHit = true;
				}
			}
			else
			{
				if (!bPlayer1IsHit)
				{
					Player1->Health -= Player2->Attacks[Player2->attackType].Damage;
					Player1->State = AFighterPawn::STATE::Stunned;
					Player1->currentFrameOfAttack = Player2->Attacks[Player2->attackType].StunRate;
					bPlayer1IsHit = true;
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
			Player2->SetActorLocation(Player2->GetActorLocation() + (toPlayer2 * (Player2->currentFrameOfAttack / 5)));
		}
		else
		{
			Player2->State = AFighterPawn::STATE::Idle;
			Player2->InputID = AFighterPawn::INPUT::IDLE;
			Player2->currentPartsIndex = 0;
			Player2->attackType = AFighterPawn::ATTACK_TYPE::NONE;
			bPlayer2IsHit = false;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 Angle: %f  -  Player 2 to Player 1 Distance: %f"), Angle(Player2->GetActorRightVector(), toPlayer1), (Player1->GetActorLocation() - Player2->GetActorLocation()).Size()));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("IsPlayerHit? %i"), bPlayer1IsHit));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 State: %i"), Player2->State));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 InputID: %i"), Player2->InputID));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 CurrentFrameOfAttack: %i"), Player2->currentFrameOfAttack));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Orange, FString::Printf(TEXT("Player 2 CurrentPartsIndex: %i"), Player2->currentPartsIndex));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Blue, FString::Printf(TEXT("Player 2 AttackType: %i"), Player2->attackType));
													   
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::White, FString::Printf(TEXT("FrameTime: %f----------------------FrameRate: %i"),DeltaTime,1000/DeltaTime));
	
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("Player 1 Angle: %f  -  Player 1 to Player 2 Distance: %f"), Angle(Player1->GetActorRightVector(), toPlayer2), (Player2->GetActorLocation() - Player1->GetActorLocation()).Size()));
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("IsPlayerHit? %i"), bPlayer2IsHit));
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

