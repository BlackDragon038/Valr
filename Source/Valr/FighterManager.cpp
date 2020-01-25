// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"
#include "GameFramework/PlayerController.h"

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
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetViewTarget(Camera);
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	Camera->SetActorLocation(FVector((Player1->GetActorLocation().X + Player2->GetActorLocation().X) * 0.5f, (Player1->GetActorLocation().Y + Player2->GetActorLocation().Y) * 0.5f, Player1->GetActorLocation().Z + 130));
	Camera->SpringArm->TargetArmLength = FVector::Distance(Player1->GetActorLocation(), Player2->GetActorLocation());
	if (Camera->SpringArm->TargetArmLength < 400) Camera->SpringArm->TargetArmLength = 400;
	if (Player1->State == AFighterPawn::STATE::Moving) Player1->SetActorRotation((Player2->GetActorLocation() - Player1->GetActorLocation()).Rotation());
	if (Player2->State == AFighterPawn::STATE::Moving) Player2->SetActorRotation((Player1->GetActorLocation() - Player2->GetActorLocation()).Rotation());

	FVector MiddleVector = Player2->GetActorLocation() - Player1->GetActorLocation();
	FVector PerpendicularVector = { MiddleVector.Y,-MiddleVector.X,MiddleVector.Z };
	PerpendicularVector.Normalize();
	Camera->SetActorRotation(PerpendicularVector.Rotation());



}

