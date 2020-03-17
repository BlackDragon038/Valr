// Code by Sultan Iljasov, 2020.

#include "Crusader.h"

void ACrusader::BeginPlay()
{
	Super::BeginPlay();
	Name = "Crusader";
}


void ACrusader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (State == STATE::ATTACKING && attackType == ATTACK_TYPE::SPECIAL)
	{
		if (specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost)
		{
			if (healEveryFrame >= healEveryXFrame)
			{
				if (Health < 255) Health += 1;
				healEveryFrame = 0;
			} else healEveryFrame++;
			specialMeter -= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost;
			currentFrameOfAttack--;
			currentPartsIndex = 0;
			attackType = ATTACK_TYPE::SPECIAL;
			InputID = INPUT::SPECIAL;
			State = STATE::ATTACKING;
		}
	}
}

void ACrusader::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING && specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
	}
}

void ACrusader::ReleasedSpecial()
{
	if (State != STATE::BLOCKING)
	{
		State = STATE::IDLE;
		InputID = INPUT::IDLE;
		currentFrameOfAttack = 0;
		currentPartsIndex = 0;
		attackType = ATTACK_TYPE::NONE;
	}
}
