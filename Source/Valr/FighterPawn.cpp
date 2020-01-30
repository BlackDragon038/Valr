// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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
	attackType = ATTACK_TYPE::NONE;
	unsigned int totalFrames = 0;
	for (auto& a : Attacks)
	{
		totalFrames = 0;
		for (auto& p : a.Parts)
		{
			totalFrames += p.Hold;
			p.PSum = totalFrames;
		}
		a.AttackTotalFrameCount = totalFrames;
	}
}

// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (W_Key == 1 && A_Key == 0 && S_Key == 0 && D_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::UP;
	else if (W_Key == 1 && A_Key == 1 && S_Key == 0 && D_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::UP_LEFT;
	else if (A_Key == 1 && W_Key == 0 && S_Key == 0 && D_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::LEFT;
	else if (A_Key == 1 && S_Key == 1 && W_Key == 0 && D_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::LEFT_DOWN;
	else if (S_Key == 1 && W_Key == 0 && D_Key == 0 && A_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::DOWN;
	else if (S_Key == 1 && D_Key == 1 && W_Key == 0 && A_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::DOWN_RIGHT;
	else if (D_Key == 1 && W_Key == 0 && A_Key == 0 && S_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::RIGHT;
	else if (D_Key == 1 && W_Key == 1 && A_Key == 0 && S_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::RIGHT_UP;
	else if (W_Key == 0 && A_Key == 0 && S_Key == 0 && D_Key == 0 && State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned)
	{
		State = STATE::Idle;
		InputID = INPUT::IDLE;
	}

	if (State != STATE::Attacking && State != STATE::Blocking && Stamina < 255) Stamina += 1;
	/*FVector EastVector(std::sin(FMath::DegreesToRadians(Angle)), std::cos(FMath::DegreesToRadians(Angle)), 0);
	float offsetAngle = FMath::RadiansToDegrees(std::acos(FVector::DotProduct(EastVector, GetActorRightVector()) / GetActorRightVector().Size() * EastVector.Size()));
	UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), offsetAngle);
	currentVector = FVector(GetActorRightVector().X *std::sin(FMath::DegreesToRadians(Angle)), GetActorRightVector().Y * std::cos(FMath::DegreesToRadians(Angle)),0);*/
}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Player1_W", this, &AFighterPawn::PressedW);
	PlayerInputComponent->BindAxis("Player1_A", this, &AFighterPawn::PressedA);
	PlayerInputComponent->BindAxis("Player1_S", this, &AFighterPawn::PressedS);
	PlayerInputComponent->BindAxis("Player1_D", this, &AFighterPawn::PressedD);
	PlayerInputComponent->BindAction("Player1_Light", IE_Pressed,  this, &AFighterPawn::PressedLight);
	PlayerInputComponent->BindAction("Player1_Medium", IE_Pressed,  this, &AFighterPawn::PressedMedium);
	PlayerInputComponent->BindAction("Player1_Heavy", IE_Pressed,  this, &AFighterPawn::PressedHeavy);
	PlayerInputComponent->BindAction("Player1_Special", IE_Pressed,  this, &AFighterPawn::PressedSpecial);
	PlayerInputComponent->BindAction("Player1_Block", IE_Pressed, this, &AFighterPawn::PressedBlock);
	PlayerInputComponent->BindAction("Player1_Block", IE_Released, this, &AFighterPawn::ReleasedBlock);
}

void AFighterPawn::PressedW(float Axis)  
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * ((isFirstPlayer) ? -MovementSpeed : MovementSpeed));
		}
		W_Key = static_cast<bool>(Axis);
	}
}

void AFighterPawn::PressedA(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((isFirstPlayer) ? -MovementSpeed : MovementSpeed));
		}
		A_Key = static_cast<bool>(Axis);
	}
}

void AFighterPawn::PressedS(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * ((isFirstPlayer) ? MovementSpeed : -MovementSpeed));
		}
		S_Key = static_cast<bool>(Axis);
	}
}

void AFighterPawn::PressedD(float Axis) 
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((isFirstPlayer) ? MovementSpeed : -MovementSpeed));
		}
		D_Key = static_cast<bool>(Axis);
	}
}

void AFighterPawn::PressedLight() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 20)
	{
		State = STATE::Attacking;
		InputID = INPUT::Light;
		attackType = ATTACK_TYPE::LIGHT;
		Stamina -= 20;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedMedium() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 45)
	{
		State = STATE::Attacking;
		InputID = INPUT::Medium;
		attackType = ATTACK_TYPE::MEDIUM;
		Stamina -= 45;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedHeavy() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 70)
	{
		State = STATE::Attacking;
		InputID = INPUT::Heavy;
		attackType = ATTACK_TYPE::HEAVY;
		Stamina -= 70;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedSpecial() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 100)
	{
		State = STATE::Attacking;
		InputID = INPUT::Special;
		attackType = ATTACK_TYPE::SPECIAL;
		Stamina -= 100;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedBlock()
{
	if (State != STATE::Attacking && State != STATE::Stunned)
	{
		State = STATE::Blocking;
		InputID = INPUT::Block;
		Stamina -= 25;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::ReleasedBlock()
{
	if (State != STATE::Attacking && State != STATE::Stunned)
	{
		State = STATE::Idle;
		InputID = INPUT::IDLE;
	}
}