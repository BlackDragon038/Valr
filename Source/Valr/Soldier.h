// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Soldier.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ASoldier : public AFighterPawn
{
	GENERATED_BODY()
	
	void BeginPlay() override;

};
