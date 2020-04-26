// Code by Sultan Iljasov, 2020.


#include "Janissary.h"

void AJanissary::BeginPlay()
{
	Super::BeginPlay();
	Name = "Janissary";
}

void AJanissary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSuperCharged)
	{
		if (specialMeter >= specialDrainSpeed)
		{
			specialMeter -= specialDrainSpeed;
			Attacks[0].Damage += 10;
			Attacks[1].Damage += 10;
			Attacks[2].Damage += 10;
		}
		else
		{
			bSuperCharged = false;
			Attacks[0].Damage -= 10;
			Attacks[1].Damage -= 10;
			Attacks[2].Damage -= 10;
		}
	}
}

void AJanissary::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING && !bNonStunnable && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		bSuperCharged = true;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
