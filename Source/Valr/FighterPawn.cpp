// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"
#include "DrawDebugHelpers.h"

#include <cmath>


// Sets default values
AFighterPawn::AFighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFighterPawn::BeginPlay()
{
	Super::BeginPlay();
	State = STATE::Idle;
	InputID = INPUT::IDLE;
}

// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AxisW == 1 && AxisA == 0 && AxisS == 0 && AxisD == 0) InputID = INPUT::UP;
	else if (AxisW == 1 && AxisA == -1 && AxisS == 0 && AxisD == 0) InputID = INPUT::UP_LEFT;
	else if (AxisA == -1 && AxisW == 0 && AxisS == 0 && AxisD == 0) InputID = INPUT::LEFT;
	else if (AxisA == -1 && AxisS == -1 && AxisW == 0 && AxisD == 0) InputID = INPUT::LEFT_DOWN;
	else if (AxisS == -1 && AxisW == 0 && AxisD == 0 && AxisA == 0) InputID = INPUT::DOWN;
	else if (AxisS == -1 && AxisD == 1 && AxisW == 0 && AxisA == 0) InputID = INPUT::DOWN_RIGHT;
	else if (AxisD == 1 && AxisW == 0 && AxisA == 0 && AxisS == 0) InputID = INPUT::RIGHT;
	else if (AxisD == 1 && AxisW == 1 && AxisA == 0 && AxisS == 0) InputID = INPUT::RIGHT_UP;
	else if (AxisW == 0 && AxisA == 0 && AxisS == 0 && AxisD == 0)
	{
		State = STATE::Idle;
		InputID = INPUT::IDLE;
	}
	FVector EastVector(std::sin(FMath::DegreesToRadians(Angle)), std::cos(FMath::DegreesToRadians(Angle)), 0);
	float offsetAngle = FMath::RadiansToDegrees(std::acos(FVector::DotProduct(EastVector, GetActorRightVector()) / GetActorRightVector().Size() * EastVector.Size()));
	UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), offsetAngle);
	currentVector = FVector(GetActorRightVector().X *std::sin(FMath::DegreesToRadians(Angle)), GetActorRightVector().Y * std::cos(FMath::DegreesToRadians(Angle)),0);

	if (isFirstPlayer) DrawDebugLine
	(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (currentVector * 100),
		FColor::Red,
		false,
		1,
		1,
		1);
}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	/*PlayerInputComponent->BindAction("W", IE_Pressed, this, &AFighterPawn::PressedW);
	PlayerInputComponent->BindAction("A", IE_Pressed, this, &AFighterPawn::PressedA);
	PlayerInputComponent->BindAction("S", IE_Pressed, this, &AFighterPawn::PressedS);
	PlayerInputComponent->BindAction("D", IE_Pressed, this, &AFighterPawn::PressedD);*/
	PlayerInputComponent->BindAxis("W", this, &AFighterPawn::PressedW);
	PlayerInputComponent->BindAxis("A", this, &AFighterPawn::PressedA);
	PlayerInputComponent->BindAxis("S", this, &AFighterPawn::PressedS);
	PlayerInputComponent->BindAxis("D", this, &AFighterPawn::PressedD);
	PlayerInputComponent->BindAction("Light", IE_Pressed,  this, &AFighterPawn::PressedLight);
	PlayerInputComponent->BindAction("Medium", IE_Pressed,  this, &AFighterPawn::PressedMedium);
	PlayerInputComponent->BindAction("Heavy", IE_Pressed,  this, &AFighterPawn::PressedHeavy);
	PlayerInputComponent->BindAction("Special", IE_Pressed,  this, &AFighterPawn::PressedSpecial);
}

void AFighterPawn::PressedW(float Axis)  
{
	if (Axis > 0) 
	{ 
		State = STATE::Moving;
		SetActorLocation(GetActorLocation() + GetActorRightVector() * ((isFirstPlayer) ? -4 : 4));
	}
	AxisW = static_cast<int8>(Axis);
}

void AFighterPawn::PressedA(float Axis)
{
	if (Axis < 0)
	{
		State = STATE::Moving;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((isFirstPlayer) ? -4 : 4));
	}
	AxisA = static_cast<int8>(Axis);
}

void AFighterPawn::PressedS(float Axis)
{
	if (Axis < 0)
	{
		State = STATE::Moving;
		SetActorLocation(GetActorLocation() + GetActorRightVector() * ((isFirstPlayer) ? 4 : -4));
	}
	AxisS = static_cast<int8>(Axis);
}

void AFighterPawn::PressedD(float Axis) 
{
	if (Axis > 0) 
	{ 
		State = STATE::Moving;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((isFirstPlayer) ? 4 : -4)); 
	}
	AxisD = static_cast<int8>(Axis);
}

void AFighterPawn::PressedLight() 
{
	UE_LOG(LogTemp, Warning, TEXT("Light Attack"));
	State = STATE::Attacking;
}

void AFighterPawn::PressedMedium() 
{
	UE_LOG(LogTemp, Warning, TEXT("Medium Attack"));
	State = STATE::Attacking;
}

void AFighterPawn::PressedHeavy() 
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));
	State = STATE::Attacking;
}

void AFighterPawn::PressedSpecial() 
{
	UE_LOG(LogTemp, Warning, TEXT("Special Attack"));
	State = STATE::Attacking;
}