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
	if (UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::UP;
	else if (UP_Key == 1 && LEFT_Key == 1 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::UP_LEFT;
	else if (LEFT_Key == 1 && UP_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::LEFT;
	else if (LEFT_Key == 1 && DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::LEFT_DOWN;
	else if (DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && LEFT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::DOWN;
	else if (DOWN_Key == 1 && RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::DOWN_RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && 
		InputID != INPUT::Block && InputID != INPUT::Special && InputID != INPUT::Heavy && InputID != INPUT::Medium && InputID != INPUT::Light && State != STATE::Stepping) InputID = INPUT::RIGHT_UP;
	else if (UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && State != STATE::Stepping)
	{
		State = STATE::Idle;
		InputID = INPUT::IDLE;
	}

	if (State != STATE::Attacking && State != STATE::Blocking && Stamina < 255 && State != STATE::Stepping)
		Stamina += 1;

	if (steppingFrameTime > 0)
	{
		steppingFrameTime--;
		if (KeyW == KEY_STATE::PRESSED_TWICE && !bDoubleTapW)
		{
			steppingSpeed = 200;
			bDoubleTapW = true;
			Stamina -= 100;
		}
		if (KeyA == KEY_STATE::PRESSED_TWICE && !bDoubleTapA)
		{
			steppingSpeed = 200;
			bDoubleTapA = true;
			Stamina -= 100;
		}
		if (KeyS == KEY_STATE::PRESSED_TWICE && !bDoubleTapS)
		{
			steppingSpeed = 200;
			bDoubleTapS = true;
			Stamina -= 100;
		}
		if (KeyD == KEY_STATE::PRESSED_TWICE && !bDoubleTapD)
		{
			steppingSpeed = 200;
			bDoubleTapD = true;
			Stamina -= 100;
		}
	}
	else
	{
		KeyW = KEY_STATE::RESET;
		KeyA = KEY_STATE::RESET;
		KeyS = KEY_STATE::RESET;
		KeyD = KEY_STATE::RESET;
	}

	if (bDoubleTapW)
	{
		if (steppingSpeed >= sideStepSpeed)
		{
			State = STATE::Stepping;
			steppingSpeed -= sideStepSpeed;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * -((float)steppingSpeed / 10.f));
		}
		else
		{
			bDoubleTapW = false;
			State = STATE::Idle;
			KeyW = KEY_STATE::RESET;
			UP_Key = false;
		}
	}
	else if (bDoubleTapA)
	{
		if (steppingSpeed >= sideStepSpeed)
		{
			State = STATE::Stepping;
			steppingSpeed -= sideStepSpeed;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * -((float)steppingSpeed / 10.f));
		}
		else
		{
			bDoubleTapA = false;
			State = STATE::Idle;
			KeyA = KEY_STATE::RESET;
			LEFT_Key = false;
		}
	}
	else if (bDoubleTapS)
	{
		if (steppingSpeed >= sideStepSpeed)
		{
			State = STATE::Stepping;
			steppingSpeed -= sideStepSpeed;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * ((float)steppingSpeed / 10.f));
		}
		else
		{
			bDoubleTapS = false;
			State = STATE::Idle;
			KeyS = KEY_STATE::RESET;
			DOWN_Key = false;
		}
	}
	else if (bDoubleTapD)
	{
		if (steppingSpeed >= sideStepSpeed)
		{
			State = STATE::Stepping;
			steppingSpeed -= sideStepSpeed;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((float)steppingSpeed / 10.f));
		}
		else
		{
			bDoubleTapD = false;
			State = STATE::Idle;
			KeyD = KEY_STATE::RESET;
			RIGHT_Key = false;
		}
	}
}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighterPawn::AxisW(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		UP_Key = static_cast<bool>(Axis);

		if (UP_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorRightVector() * -MovementSpeed);
			State = STATE::Moving;
		}

		if (InputID == INPUT::UP && Stamina > 100)
		{
			if (KeyW == KEY_STATE::RESET && UP_Key)
			{
				KeyW = KEY_STATE::PRESSED_ONCE;
				steppingFrameTime = 10;
			}
			else if (KeyW == KEY_STATE::PRESSED_ONCE && !UP_Key)
				KeyW = KEY_STATE::RELEASED;
			else if (KeyW == KEY_STATE::RELEASED && UP_Key)
			{
				KeyW = KEY_STATE::PRESSED_TWICE;
			}
		}
	}
}

void AFighterPawn::AxisA(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping && Stamina > 25)
	{
		LEFT_Key = static_cast<bool>(Axis);

		if (LEFT_Key)
		{
			Stamina -= 2;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * -MovementSpeed);
			State = STATE::Moving;
		}

		if (InputID == INPUT::LEFT && Stamina > 100)
		{
			if (KeyA == KEY_STATE::RESET && LEFT_Key)
			{
				KeyA = KEY_STATE::PRESSED_ONCE;
				steppingFrameTime = 10;
			}
			else if (KeyA == KEY_STATE::PRESSED_ONCE && !LEFT_Key)
				KeyA = KEY_STATE::RELEASED;
			else if (KeyA == KEY_STATE::RELEASED && LEFT_Key)
			{
				KeyA = KEY_STATE::PRESSED_TWICE;
			}
		}
	}
}

void AFighterPawn::AxisS(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		DOWN_Key = static_cast<bool>(Axis);

		if (DOWN_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorRightVector() * MovementSpeed);
			State = STATE::Moving;
		}

		if (InputID == INPUT::DOWN && Stamina > 100)
		{
			if (KeyS == KEY_STATE::RESET && DOWN_Key)
			{
				KeyS = KEY_STATE::PRESSED_ONCE;
				steppingFrameTime = 10;
			}
			else if (KeyS == KEY_STATE::PRESSED_ONCE && !DOWN_Key)
				KeyS = KEY_STATE::RELEASED;
			else if (KeyS == KEY_STATE::RELEASED && DOWN_Key)
			{
				KeyS = KEY_STATE::PRESSED_TWICE;
			}
		}
	}
}

void AFighterPawn::AxisD(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		RIGHT_Key = static_cast<bool>(Axis);

		if (RIGHT_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed);
			State = STATE::Moving;
		}

		if (InputID == INPUT::RIGHT && Stamina > 100)
		{
			if (KeyD == KEY_STATE::RESET && RIGHT_Key)
			{
				KeyD = KEY_STATE::PRESSED_ONCE;
				steppingFrameTime = 10;
			}
			else if (KeyD == KEY_STATE::PRESSED_ONCE && !RIGHT_Key)
				KeyD = KEY_STATE::RELEASED;
			else if (KeyD == KEY_STATE::RELEASED && RIGHT_Key)
			{
				KeyD = KEY_STATE::PRESSED_TWICE;
			}
		}
	}
}

void AFighterPawn::PressedW()  
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		UP_Key = true;
		DOWN_Key = false;
		if (KeyW == KEY_STATE::RESET)
		{
			KeyW = KEY_STATE::PRESSED_ONCE;
			steppingFrameTime = 15;
		}
		else if (KeyW == KEY_STATE::PRESSED_ONCE)
		{
			KeyW = KEY_STATE::PRESSED_TWICE;
		}
	}
}

void AFighterPawn::PressedA()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		LEFT_Key = true;
		RIGHT_Key = false;
		if (KeyA == KEY_STATE::RESET)
		{
			KeyA = KEY_STATE::PRESSED_ONCE;
			steppingFrameTime = 15;
		}
		else if (KeyA == KEY_STATE::PRESSED_ONCE)
		{
			KeyA = KEY_STATE::PRESSED_TWICE;
		}
	}
}

void AFighterPawn::PressedS()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		DOWN_Key = true;
		UP_Key = false;
		if (KeyS == KEY_STATE::RESET)
		{
			KeyS = KEY_STATE::PRESSED_ONCE;
			steppingFrameTime = 15;
		}
		else if (KeyS == KEY_STATE::PRESSED_ONCE)
		{
			KeyS = KEY_STATE::PRESSED_TWICE;
		}
	}
}

void AFighterPawn::PressedD() 
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		RIGHT_Key = true;
		LEFT_Key = false;
		if (KeyD == KEY_STATE::RESET)
		{
			KeyD = KEY_STATE::PRESSED_ONCE;
			steppingFrameTime = 15;
		}
		else if (KeyD == KEY_STATE::PRESSED_ONCE)
		{
			KeyD = KEY_STATE::PRESSED_TWICE;
		}
	}
}

void AFighterPawn::ReleasedW()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		UP_Key = false;
	}
}

void AFighterPawn::ReleasedA()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		LEFT_Key = false;
	}
}

void AFighterPawn::ReleasedS()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		DOWN_Key = false;
	}
}

void AFighterPawn::ReleasedD()
{
	if (State != STATE::Attacking && State != STATE::Blocking && State != STATE::Stunned && Stamina > 10 && State != STATE::Stepping)
	{
		RIGHT_Key = false;
	}
}

void AFighterPawn::PressedLight() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 20 && State != STATE::Stepping)
	{
		State = STATE::Attacking;
		InputID = INPUT::Light;
		attackType = ATTACK_TYPE::LIGHT;
		if (Stamina > 20) Stamina -= 20;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedMedium() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 45 && State != STATE::Stepping)
	{
		State = STATE::Attacking;
		InputID = INPUT::Medium;
		attackType = ATTACK_TYPE::MEDIUM;
		if (Stamina > 45) Stamina -= 45;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedHeavy() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking && Stamina >= 70 && State != STATE::Stepping)
	{
		State = STATE::Attacking;
		InputID = INPUT::Heavy;
		attackType = ATTACK_TYPE::HEAVY;
		if (Stamina > 70) Stamina -= 70;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedSpecial() 
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Blocking &&  Stamina >= 100 && State != STATE::Stepping)
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

void AFighterPawn::AxisBlock(float Axis)
{
	if (State != STATE::Attacking && State != STATE::Stunned && State != STATE::Stepping)
	{
		if (Axis > 0)
		{
			State = STATE::Blocking;
			InputID = INPUT::Block;
			if (InputID != INPUT::Block)
				if (Stamina > 25) 
						Stamina -= 25;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
		}
		else if (Axis < 1)
		{
			if (InputID == INPUT::Block)
			{
				State = STATE::Idle;
				InputID = INPUT::IDLE;
			}
		}
	}
}
