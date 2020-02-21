// Code by Sultan Iljasov, 2020.


#include "Viking.h"

void AViking::BeginPlay()
{
	Super::BeginPlay();
	Name = "Viking";
}

void AViking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bNonStunnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("VIKING ENRAGED!"))
		if (specialMeter >= specialDrainSpeed)
			specialMeter -= specialDrainSpeed;
		else
		{
			bNonStunnable = false;
		}
	}
}

void AViking::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		bNonStunnable = true;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
