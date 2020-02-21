// Code by Sultan Iljasov, 2020.


#include "Samurai.h"

void ASamurai::BeginPlay()
{
	Super::BeginPlay();
	Name = "Samurai";
}

void ASamurai::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (attackType == ATTACK_TYPE::SPECIAL && State != STATE::STUNNED && State != STATE::IDLE) 
		if (GetActorLocation().Size() < maxDistanceFromCenter - Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount - currentFrameOfAttack) 
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * (Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount - currentFrameOfAttack) * specialAttackSpeedMultiplier);
}

void ASamurai::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
