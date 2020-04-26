// Code by Sultan Iljasov, 2020.


#include "Ninja.h"
#include "FightManager.h"

void ANinja::BeginPlay()
{
	Super::BeginPlay();
	Name = "Ninja";
}

void ANinja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (attackType == ATTACK_TYPE::SPECIAL)
	{
		if (Manager->Player1 == this)
		{
			if (currentFrameOfAttack == Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount / 2)
			{
				FVector Direction(GetActorLocation() - Manager->Player2->GetActorLocation());
				Direction.Normalize();
				SetActorLocation(Manager->Player2->GetActorLocation() + (Direction * -100));
				SetActorRotation((GetActorForwardVector() * -1).Rotation());
				Manager->Camera->SpringArm->TargetArmLength = Manager->defaultDistanceBetweenPlayers * Manager->Camera->closestDistance;
			}
		}
		else
		{
			if (currentFrameOfAttack == Attacks[static_cast<uint8>(attackType)].AttackTotalFrameCount / 2)
			{
				FVector Direction(GetActorLocation() - Manager->Player1->GetActorLocation());
				Direction.Normalize();
				SetActorLocation(Manager->Player1->GetActorLocation() + (Direction * -100));
				SetActorRotation((GetActorForwardVector() * -1).Rotation());
				Manager->Camera->SpringArm->TargetArmLength = Manager->defaultDistanceBetweenPlayers * Manager->Camera->closestDistance;
			}
		}
	}
}

void ANinja::PressedSpecial()
{
	if (State != STATE::ATTACKING && State != STATE::STUNNED && State != STATE::BLOCKING &&  specialMeter >= Attacks[static_cast<uint8>(ATTACK_TYPE::SPECIAL)].staminaCost && State != STATE::STEPPING && !bDisableInput)
	{
		State = STATE::ATTACKING;
		InputID = INPUT::SPECIAL;
		attackType = ATTACK_TYPE::SPECIAL;
		if (specialMeter >= Attacks[static_cast<uint8>(attackType)].staminaCost) specialMeter -= Attacks[static_cast<uint8>(attackType)].staminaCost;	//NOTE: If this is called in an AI controller, it will drain stamina faster than you can say fuck.
	}
}
