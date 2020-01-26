// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include <cmath>

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
	
}

float AFighterManager::Angle(FVector a, FVector b)
{
	return FMath::RadiansToDegrees(std::acos(FVector::DotProduct(a, b) / a.Size() * b.Size()));
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	FVector toPlayer1 = Player1->GetActorLocation() - Player2->GetActorLocation();
	toPlayer1.Normalize();
	FVector toPlayer2 = Player2->GetActorLocation() - Player1->GetActorLocation();
	toPlayer2.Normalize();
	Camera->SetActorLocation(FVector((Player1->GetActorLocation().X + Player2->GetActorLocation().X) * 0.5f, (Player1->GetActorLocation().Y + Player2->GetActorLocation().Y) * 0.5f, Player1->GetActorLocation().Z + 130));
	Camera->SpringArm->TargetArmLength = FVector::Distance(Player1->GetActorLocation(), Player2->GetActorLocation());
	if (Camera->SpringArm->TargetArmLength < 400) Camera->SpringArm->TargetArmLength = 400;
	if (Player1->State == AFighterPawn::STATE::Moving) Player1->SetActorRotation(toPlayer2.Rotation());
	if (Player2->State == AFighterPawn::STATE::Moving) Player2->SetActorRotation(toPlayer1.Rotation());

	FVector MiddleVector = Player2->GetActorLocation() - Player1->GetActorLocation();
	FVector PerpendicularVector = { MiddleVector.Y,-MiddleVector.X,MiddleVector.Z };
	PerpendicularVector.Normalize();
	Camera->SetActorRotation(PerpendicularVector.Rotation());
	
	if (Player1->State == AFighterPawn::STATE::Attacking)
	{
		if (Player1->currentFrameOfAttack > Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].TotalFrameCount)
		{
			if (Player1->currentPartsIndex < Player1->Attacks[Player1->attackType].Parts.Num()-1)
				Player1->currentPartsIndex++;
		}
		
		if (Angle(Player1->GetActorRightVector(), toPlayer2) > Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minAngle &&
			Angle(Player1->GetActorRightVector(), toPlayer2) < Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxAngle &&
		   (Player2->GetActorLocation() - Player1->GetActorLocation()).Size() > Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].minDist &&
		   (Player2->GetActorLocation() - Player1->GetActorLocation()).Size() < Player1->Attacks[Player1->attackType].Parts[Player1->currentPartsIndex].maxAngle)
		{
			UE_LOG(LogTemp,Error,TEXT("HIT!!!!!!!"))
			bPlayer2IsHit = true;
			Player1->State = AFighterPawn::STATE::Idle;
			Player1->InputID = AFighterPawn::INPUT::IDLE;
			Player1->currentFrameOfAttack = 0;
			Player1->currentPartsIndex = 0;
			Player1->attackType = AFighterPawn::ATTACK_TYPE::NONE;
		}
		else Player1->currentFrameOfAttack++;
	}
	UE_LOG(LogTemp, Warning, TEXT("Angle: %f/nDistance: %f"), Angle(Player1->GetActorRightVector(), toPlayer2), (Player2->GetActorLocation() - Player1->GetActorLocation()).Size())
	if (Player1->isFirstPlayer) DrawDebugLine
	(
		GetWorld(),
		Player2->GetActorLocation(),
		Player2->GetActorLocation() + (Player2->GetActorRightVector() * 100),
		FColor::Red,
		false,
		1,
		1,
		1);

	if (Player1->isFirstPlayer) DrawDebugLine
	(
		GetWorld(),
		Player2->GetActorLocation(),
		Player1->GetActorLocation() - Player2->GetActorLocation(),
		FColor::Red,
		false,
		1,
		1,
		1);
}

