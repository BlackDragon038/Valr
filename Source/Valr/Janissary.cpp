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
			if (!bBoosted)
			{
				bBoosted = true;
				Damages[0] = Attacks[0].Damage;
				Damages[1] = Attacks[1].Damage;
				Damages[2] = Attacks[2].Damage;
				
				Attacks[0].Damage *= 1.15f;
				Attacks[1].Damage *= 1.15f;
				Attacks[2].Damage *= 1.15f;
			}
			specialMeter -= specialDrainSpeed;
		}
		else
		{
			bBoosted = false;
			bSuperCharged = false;
			Attacks[0].Damage = Damages[0];
			Attacks[1].Damage = Damages[1];
			Attacks[2].Damage = Damages[2];
		}
	}
}

void AJanissary::PressedSpecial()
{
	if (!bBoosted && State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING && !bNonStunnable && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		bSuperCharged = true;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
