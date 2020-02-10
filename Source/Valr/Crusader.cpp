// Fill out your copyright notice in the Description page of Project Settings.


#include "Crusader.h"

void ACrusader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACrusader::PressedSpecial()
{
	UE_LOG(LogTemp,Warning,TEXT("Crusader used SPECIAL!"))
	inputBufferKey = INPUT::SPECIAL;
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  Stamina >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), specialAttackParticle, GetActorLocation() + FVector(0, 0, 0), FRotator(0));
		if (Stamina > Attacks[static_cast<uint8>(attackType)].staminaCost) Stamina -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
