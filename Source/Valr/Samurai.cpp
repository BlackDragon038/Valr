// Fill out your copyright notice in the Description page of Project Settings.


#include "Samurai.h"

void ASamurai::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (attackType == ATTACK_TYPE::SPECIAL && State != STATE::STUNNED) SetActorLocation(GetActorLocation() + GetActorForwardVector() * (Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount-currentFrameOfAttack));
}

void ASamurai::PressedSpecial()
{
	UE_LOG(LogTemp, Warning, TEXT("Samurai used SPECIAL!"))
	inputBufferKey = INPUT::SPECIAL;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
