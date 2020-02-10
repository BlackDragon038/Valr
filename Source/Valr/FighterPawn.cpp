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

	State = STATE::IDLE;
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
		inputBuffer[0] = inputBuffer[1];
		inputBuffer[1] = inputBuffer[2];
		inputBuffer[2] = inputBuffer[3];
		inputBuffer[3] = inputBuffer[4];
		inputBuffer[4] = inputBuffer[5];
		inputBuffer[5] = inputBuffer[6];
		inputBuffer[6] = inputBuffer[7];
		inputBuffer[7] = inputBuffer[8];
		inputBuffer[8] = inputBuffer[9];
		inputBuffer[9] = inputBufferKey;
		//UE_LOG(LogTemp, Warning, TEXT("%i %i %i %i %i %i %i %i %i"),inputBuffer[0], inputBuffer[1], inputBuffer[2], inputBuffer[3], inputBuffer[4], inputBuffer[5], inputBuffer[6], inputBuffer[7], inputBuffer[8], inputBuffer[9])
	if (UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::UP;
	else if (UP_Key == 1 && LEFT_Key == 1 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::UP_LEFT;
	else if (LEFT_Key == 1 && UP_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::LEFT;
	else if (LEFT_Key == 1 && DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::LEFT_DOWN;
	else if (DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && LEFT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::DOWN;
	else if (DOWN_Key == 1 && RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::DOWN_RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && 
		InputID != INPUT::BLOCK && InputID != INPUT::SPECIAL && InputID != INPUT::HEAVY && InputID != INPUT::MEDIUM && InputID != INPUT::LIGHT && State != STATE::STEPPING) InputID = INPUT::RIGHT_UP;
	else if (UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING)
	{
		State = STATE::IDLE;
		InputID = INPUT::IDLE;
	}

	if (State != STATE::ATTACKING && State != STATE::BLOCKING && Stamina < 255-staminaRegeneration && State != STATE::STEPPING)
		Stamina += staminaRegeneration;

	if (steppingFrameTime > 0)
	{
		steppingFrameTime--;
		if (KeyW == KEY_STATE::PRESSED_TWICE && !bDoubleTapW)
		{
			steppingSpeed = sideStepFrameTime;
			bDoubleTapW = true;
			Stamina -= 100;
		}
		if (KeyA == KEY_STATE::PRESSED_TWICE && !bDoubleTapA)
		{
			steppingSpeed = sideStepFrameTime;
			bDoubleTapA = true;
			Stamina -= 100;
		}
		if (KeyS == KEY_STATE::PRESSED_TWICE && !bDoubleTapS)
		{
			steppingSpeed = sideStepFrameTime;
			bDoubleTapS = true;
			Stamina -= 100;
		}
		if (KeyD == KEY_STATE::PRESSED_TWICE && !bDoubleTapD)
		{
			steppingSpeed = sideStepFrameTime;
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
		if (steppingSpeed > 0)
		{
			State = STATE::STEPPING;
			steppingSpeed -= 1;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * -((float)steppingSpeed * sideStepSpeed / 100.f));
		}
		else
		{
			bDoubleTapW = false;
			State = STATE::IDLE;
			KeyW = KEY_STATE::RESET;
			UP_Key = false;
		}
	}
	else if (bDoubleTapA)
	{
		if (steppingSpeed > 0)
		{
			State = STATE::STEPPING;
			steppingSpeed -= 1;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * -((float)steppingSpeed * sideStepSpeed / 100.f));
		}
		else
		{
			bDoubleTapA = false;
			State = STATE::IDLE;
			KeyA = KEY_STATE::RESET;
			LEFT_Key = false;
		}
	}
	else if (bDoubleTapS)
	{
		if (steppingSpeed > 0)
		{
			State = STATE::STEPPING;
			steppingSpeed -= 1;
			SetActorLocation(GetActorLocation() + GetActorRightVector() * ((float)steppingSpeed * sideStepSpeed / 100.f));
		}
		else
		{
			bDoubleTapS = false;
			State = STATE::IDLE;
			KeyS = KEY_STATE::RESET;
			DOWN_Key = false;
		}
	}
	else if (bDoubleTapD)
	{
		if (steppingSpeed > 0)
		{
			State = STATE::STEPPING;
			steppingSpeed -= 1;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * ((float)steppingSpeed * sideStepSpeed / 100.f));
		}
		else
		{
			bDoubleTapD = false;
			State = STATE::IDLE;
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		UP_Key = static_cast<bool>(Axis);

		if (UP_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorRightVector() * -MovementSpeed);
			State = STATE::MOVING;
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING && Stamina > 25)
	{
		LEFT_Key = static_cast<bool>(Axis);

		if (LEFT_Key)
		{
			Stamina -= 3;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * -MovementSpeed);
			State = STATE::MOVING;
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		DOWN_Key = static_cast<bool>(Axis);

		if (DOWN_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorRightVector() * MovementSpeed);
			State = STATE::MOVING;
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		RIGHT_Key = static_cast<bool>(Axis);

		if (RIGHT_Key)
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed);
			State = STATE::MOVING;
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
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
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		UP_Key = false;
	}
}

void AFighterPawn::ReleasedA()
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		LEFT_Key = false;
	}
}

void AFighterPawn::ReleasedS()
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		DOWN_Key = false;
	}
}

void AFighterPawn::ReleasedD()
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && Stamina > 10 && State != STATE::STEPPING)
	{
		RIGHT_Key = false;
	}
}

void AFighterPawn::PressedLight() 
{
	inputBufferKey = INPUT::LIGHT;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::LIGHT)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::LIGHT;
		attackType = ATTACK_TYPE::LIGHT;
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedMedium() 
{
	inputBufferKey = INPUT::MEDIUM;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::MEDIUM)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::MEDIUM;
		attackType = ATTACK_TYPE::MEDIUM;
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedHeavy() 
{
	inputBufferKey = INPUT::HEAVY;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::HEAVY)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::HEAVY;
		attackType = ATTACK_TYPE::HEAVY;
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedSpecial() 
{
	inputBufferKey = INPUT::SPECIAL;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedBlock()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING)
	{
		State = STATE::BLOCKING;
		InputID = INPUT::BLOCK;
		if (Stamina > BlockData.staminaCost) Stamina -= BlockData.staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::ReleasedBlock()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING)
	{
		State = STATE::IDLE;
		InputID = INPUT::IDLE;
	}
}

void AFighterPawn::AxisBlock(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING)
	{
		if (Axis > 0 && Stamina > BlockData.staminaCost)
		{
			State = STATE::BLOCKING;
			if (InputID != INPUT::BLOCK)
					Stamina -= BlockData.staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
			InputID = INPUT::BLOCK;
		}
		else if (Axis < 1)
		{
			if (InputID == INPUT::BLOCK)
			{
				State = STATE::IDLE;
				InputID = INPUT::IDLE;
			}
		}
	}
}
