// Code by Sultan Iljasov, 2020.

#include "FighterPawn.h"
#include "Kismet/GameplayStatics.h"
#include "FightManager.h"
#include <algorithm>
#include <cmath>

template<typename T>
T Clamp(T value, T min, T max)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}

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
	inputBufferIndex = 0;
	inputBuffer.reserve(inputBufferSize);
	Name = "Base Fighter";
	Instance = Cast<UFightingGameInstance>(GetGameInstance());
	Manager = Cast<AFightManager>(GetOwner());
}

void AFighterPawn::Reset()
{
	Health = 255;
	Stamina = 0;
	InputID = INPUT::IDLE;
	UP_Key = false;
	DOWN_Key = false;
	LEFT_Key = false;
	RIGHT_Key = false;
	KeyW = KEY_STATE::RESET;
	KeyA = KEY_STATE::RESET;
	KeyS = KEY_STATE::RESET;
	KeyD = KEY_STATE::RESET;
	steppingSpeed = 0;
	steppingFrameTime = 0;
	bDoubleTapW = false;
	bDoubleTapA = false;
	bDoubleTapS = false;
	bDoubleTapD = false;
	attackType = ATTACK_TYPE::NONE;
	inputBuffer.clear();
	currentFrameOfAttack = 0;
	currentPartsIndex = 0;
	bOpponentIsHit = false;
	bResetAnimation = false;
	bCanCombo = true;
	bAllowParry = false;
}

void AFighterPawn::AttackReset()
{
	State = STATE::IDLE;
	attackType = ATTACK_TYPE::NONE;
	currentFrameOfAttack = 0;
	currentPartsIndex = 0;
}

void AFighterPawn::testSideStep()
{
	if (State != STATE::STUNNED)
	{
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
				if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - ((float)steppingSpeed * sideStepSpeed / 100.f))
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
				if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - ((float)steppingSpeed * sideStepSpeed / 100.f))
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
				if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - ((float)steppingSpeed * sideStepSpeed / 100.f))
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
				if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - ((float)steppingSpeed * sideStepSpeed / 100.f))
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
}

// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (inputBufferIndex < inputBufferSize) 
	{
		inputBuffer.push_back(INPUT::IDLE);
		inputBufferIndex++;
	}
	else
	{
		inputBuffer.erase(inputBuffer.begin());
		inputBuffer.push_back(INPUT::IDLE);
	}


	for (auto &a : inputBuffer)
	{
		UE_LOG(LogTemp, Warning, TEXT("%i"),a)
	}
	UE_LOG(LogTemp, Warning, TEXT("---------"))*/

	if (UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::UP;
	else if (UP_Key == 1 && LEFT_Key == 1 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::UP_LEFT;
	else if (LEFT_Key == 1 && UP_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::LEFT;
	else if (LEFT_Key == 1 && DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::LEFT_DOWN;
	else if (DOWN_Key == 1 && UP_Key == 0 && RIGHT_Key == 0 && LEFT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::DOWN;
	else if (DOWN_Key == 1 && RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::DOWN_RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::RIGHT;
	else if (RIGHT_Key == 1 && UP_Key == 1 && LEFT_Key == 0 && DOWN_Key == 0 && 
		State == STATE::MOVING) InputID = INPUT::RIGHT_UP;
	else if (UP_Key == 0 && LEFT_Key == 0 && DOWN_Key == 0 && RIGHT_Key == 0 && 
		State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING && State != STATE::KNOCKED_DOWN && State != STATE::GETTING_UP)
	{
		State = STATE::IDLE;
		InputID = INPUT::IDLE;
	}

	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STEPPING)
	{
		if (Stamina < 255 - staminaRegeneration)
			Stamina += staminaRegeneration;
		else
			Stamina = 255;
	}

	testSideStep();
}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighterPawn::WalkBackToPosition(FVector Pos)
{
	LEFT_Key = true;
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * -MovementSpeed);
	State = STATE::MOVING;
}

void AFighterPawn::AxisW(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput && !DOWN_Key)
	{
		if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - MovementSpeed)
		{
			UP_Key = static_cast<bool>(Axis);

			if (UP_Key)
			{
				float Speed = FVector::Distance(Manager->Player1->GetActorLocation(), Manager->Player2->GetActorLocation()) * 0.025f;
				Speed = Clamp(Speed, 2.f, 4.f);
				SetActorLocation(GetActorLocation() + GetActorRightVector() * -Speed);
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
		else
		{
			UP_Key = static_cast<bool>(Axis);
			FVector toOrigin = -GetActorLocation();
			toOrigin.Normalize();
			SetActorLocation(GetActorLocation() + toOrigin * MovementSpeed);
		}
	}
}

void AFighterPawn::AxisA(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING && Stamina > 25 && !bDisableInput && !RIGHT_Key)
	{
		if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - MovementSpeed)
		{
			LEFT_Key = static_cast<bool>(Axis);

			if (LEFT_Key)
			{
				SetActorLocation(GetActorLocation() + GetActorForwardVector() * (float)(-MovementSpeed/1.5f));
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
		else
		{
			LEFT_Key = static_cast<bool>(Axis); 
			FVector toOrigin = -GetActorLocation();
			toOrigin.Normalize();
			SetActorLocation(GetActorLocation() + toOrigin * MovementSpeed);
		}
	}
}

void AFighterPawn::AxisS(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput && !UP_Key)
	{
		if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - MovementSpeed)
		{
			DOWN_Key = static_cast<bool>(Axis);

			if (DOWN_Key)
			{
				float Speed = FVector::Distance(Manager->Player1->GetActorLocation(), Manager->Player2->GetActorLocation()) * 0.025f;
				Speed = Clamp(Speed, 2.f, 4.f);
				SetActorLocation(GetActorLocation() + GetActorRightVector() * Speed);
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
		else
		{
			DOWN_Key = static_cast<bool>(Axis);
			FVector toOrigin = -GetActorLocation();
			toOrigin.Normalize();
			SetActorLocation(GetActorLocation() + toOrigin * MovementSpeed);
		}
	}
}

void AFighterPawn::AxisD(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::BLOCKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput && !LEFT_Key)
	{
		if (GetActorLocation().Size() < Instance->maxDistanceFromCenter - MovementSpeed)
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
		else
		{
			RIGHT_Key = static_cast<bool>(Axis);
			FVector toOrigin = -GetActorLocation();
			toOrigin.Normalize();
			SetActorLocation(GetActorLocation() + toOrigin * MovementSpeed);
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
	if (State == STATE::ATTACKING && bCanCombo && attackType != ATTACK_TYPE::LIGHT && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::LIGHT)].staminaCost && !bDisableInput)
	{
		if (currentFrameOfAttack >= Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount / 2)
		{
			currentFrameOfAttack = 0;
			currentPartsIndex = 0;
			bResetAnimation = true;
			bOpponentIsHit = false;
			InputID = INPUT::LIGHT;
			attackType = ATTACK_TYPE::LIGHT;
			if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost)
				Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;
		}
		else
			bCanCombo = false;
	}
	else if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::LIGHT)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::LIGHT;
		attackType = ATTACK_TYPE::LIGHT;
		if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedMedium() 
{
	if (State == STATE::ATTACKING && bCanCombo && attackType != ATTACK_TYPE::MEDIUM &&  Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::MEDIUM)].staminaCost && !bDisableInput)
	{
		if (currentFrameOfAttack >= Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount / 2)
		{
			currentFrameOfAttack = 0;
			currentPartsIndex = 0;
			bResetAnimation = true;
			bOpponentIsHit = false;
			InputID = INPUT::MEDIUM;
			attackType = ATTACK_TYPE::MEDIUM;
			if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost)
				Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;
		}
		else
			bCanCombo = false;
	}
	else if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::MEDIUM)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::MEDIUM;
		attackType = ATTACK_TYPE::MEDIUM;
		if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedHeavy() 
{
	if (State == STATE::ATTACKING && bCanCombo && attackType != ATTACK_TYPE::HEAVY && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::HEAVY)].staminaCost && !bDisableInput)
	{
		if (currentFrameOfAttack >= Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount / 2)
		{
			currentFrameOfAttack = 0;
			currentPartsIndex = 0;
			bResetAnimation = true;
			bOpponentIsHit = false; 
			InputID = INPUT::HEAVY;
			attackType = ATTACK_TYPE::HEAVY;
			if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost)
				Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;
		}
		else
			bCanCombo = false;
	}
	else if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::HEAVY)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::HEAVY;
		attackType = ATTACK_TYPE::HEAVY;
		if (Stamina >= Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedSpecial() 
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}

void AFighterPawn::PressedBlock()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput)
	{
		if (blockCooldown == 0 && Stamina > BlockData.staminaCost)
		{
			State = STATE::BLOCKING;
			Stamina -= BlockData.staminaCost;
			InputID = INPUT::BLOCK;
			blockCooldown = BlockData.Cooldown;
		}
	}
}

void AFighterPawn::ReleasedBlock()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::IDLE;
		InputID = INPUT::IDLE;
	}
}

void AFighterPawn::AxisBlock(float Axis)
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::STEPPING && !bDisableInput)
	{
		if (Axis > 0 && blockCooldown == 0 && Stamina > BlockData.staminaCost)
		{
			State = STATE::BLOCKING;
			if (InputID != INPUT::BLOCK)
			{
				Stamina -= BlockData.staminaCost;
				if (Manager->Player1 == this)
				{
					if (Manager->Player2->State == STATE::ATTACKING)
						bAllowParry = true;
				}
				else if (Manager->Player2 == this)
				{
					if (Manager->Player1->State == STATE::ATTACKING)
						bAllowParry = true;
				}
			}
			InputID = INPUT::BLOCK;
			blockCooldown = BlockData.Cooldown;
		}
		else if (Axis < 1)
		{
			if (InputID == INPUT::BLOCK)
			{
				bAllowParry = false;
				State = STATE::IDLE;
				InputID = INPUT::IDLE;
			}
		}
	}
	if (InputID != INPUT::BLOCK && blockCooldown > 0)
		blockCooldown--;
}