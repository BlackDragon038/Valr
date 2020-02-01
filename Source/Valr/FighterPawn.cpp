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

	State = Idle;
	InputID = IDLE;
	attackType = NONE;
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
	if (UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::UP;
	else if (UP_Key == 1 && LEFT_Key == 1 && DOWN_Key == 0 && RIGHT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::UP_LEFT;
	else if (LEFT_Key == 1 && UP_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::LEFT;
	else if (LEFT_Key == 1 && DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::LEFT_DOWN;
	else if (DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && LEFT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::DOWN;
	else if (DOWN_Key == 1 && RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::DOWN_RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light) InputID = INPUT::RIGHT_UP;
	else if (UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned)
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
}

void AFighterPawn::PressedW(float Axis)  
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * -MovementSpeed);
		}
		UP_Key = static_cast<bool>(Axis);

		if (KeyW == KEY_STATE::RESET && UP_Key) KeyW = KEY_STATE::PRESSED_ONCE;
		else if (KeyW == KEY_STATE::PRESSED_ONCE && !UP_Key) KeyW = KEY_STATE::RELEASED_AFTER_PRESSING;
		else if (KeyW == KEY_STATE::RELEASED_AFTER_PRESSING && UP_Key) KeyW = KEY_STATE::PRESSED_TWICE;
		else if (KeyW == KEY_STATE::PRESSED_TWICE && !UP_Key) KeyW = KEY_STATE::RESET;
	}
}

void AFighterPawn::PressedA(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * -MovementSpeed);
			if (Stamina > 2) Stamina -= 2;
		}
		LEFT_Key = static_cast<bool>(Axis);

		if (KeyA == KEY_STATE::RESET && LEFT_Key) { KeyA = KEY_STATE::PRESSED_ONCE; UE_LOG(LogTemp, Warning, TEXT("PRESSED_ONCE")) }
		else if (KeyA == KEY_STATE::PRESSED_ONCE && !LEFT_Key) { UE_LOG(LogTemp, Warning, TEXT("RELEASED")); KeyA = KEY_STATE::RELEASED_AFTER_PRESSING; }
		else if (KeyA == KEY_STATE::RELEASED_AFTER_PRESSING && LEFT_Key) { UE_LOG(LogTemp, Warning, TEXT("PRESSED_TWICE")); KeyA = KEY_STATE::PRESSED_TWICE; }
		else if (KeyA == KEY_STATE::PRESSED_TWICE && !LEFT_Key) { UE_LOG(LogTemp, Warning, TEXT("RESET"));  KeyA = KEY_STATE::RESET; }
	}
}

void AFighterPawn::PressedS(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * MovementSpeed);
		}
		DOWN_Key = static_cast<bool>(Axis);

		if (KeyS == KEY_STATE::RESET && DOWN_Key) KeyS = KEY_STATE::PRESSED_ONCE;
		else if (KeyS == KEY_STATE::PRESSED_ONCE && !DOWN_Key) KeyS = KEY_STATE::RELEASED_AFTER_PRESSING;
		else if (KeyS == KEY_STATE::RELEASED_AFTER_PRESSING && DOWN_Key) KeyS = KEY_STATE::PRESSED_TWICE;
		else if (KeyS == KEY_STATE::PRESSED_TWICE && !DOWN_Key) KeyS = KEY_STATE::RESET;
	}
}

void AFighterPawn::PressedD(float Axis) 
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10)
	{
		if (Axis > 0)
		{
			State = STATE::Moving;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed);
		}
		RIGHT_Key = static_cast<bool>(Axis);

		if (KeyD == KEY_STATE::RESET && RIGHT_Key) KeyD = KEY_STATE::PRESSED_ONCE;
		else if (KeyD == KEY_STATE::PRESSED_ONCE && !RIGHT_Key) KeyD = KEY_STATE::RELEASED_AFTER_PRESSING;
		else if (KeyD == KEY_STATE::RELEASED_AFTER_PRESSING && RIGHT_Key) KeyD = KEY_STATE::PRESSED_TWICE;
		else if (KeyD == KEY_STATE::PRESSED_TWICE && !RIGHT_Key) KeyD = KEY_STATE::RESET;
	}
}

void AFighterPawn::PressedLight() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 20)
	{
		State = STATE::Attacking;
		InputID = INPUT::Light;
		attackType = ATTACK_TYPE::LIGHT;
		if (Stamina > 20) Stamina -= 20;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedMedium() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 45)
	{
		State = STATE::Attacking;
		InputID = INPUT::Medium;
		attackType = ATTACK_TYPE::MEDIUM;
		if (Stamina > 45) Stamina -= 45;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedHeavy() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 70)
	{
		State = STATE::Attacking;
		InputID = INPUT::Heavy;
		attackType = ATTACK_TYPE::HEAVY;
		if (Stamina > 70) Stamina -= 70;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedSpecial() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking &&  Stamina >= 100)
	{
		State = STATE::Attacking;
		InputID = INPUT::Special;
		attackType = ATTACK_TYPE::SPECIAL;
		if (Stamina > 100) Stamina -= 100;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedBlock()
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Stepping)
	{
		State = STATE::Blocking;
		InputID = INPUT::Block;
		if (Stamina > 25) Stamina -= 25;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::ReleasedBlock()
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Stepping)
	{
		State = STATE::Idle;
		InputID = INPUT::IDLE;
	}
}