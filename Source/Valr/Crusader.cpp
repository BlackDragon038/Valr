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
}

void ACrusader::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), specialAttackParticle, GetActorLocation() + FVector(0, 0, 0), FRotator(0));
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
