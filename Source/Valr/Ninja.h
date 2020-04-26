// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Ninja.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ANinja : public AFighterPawn
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	void Tick(float DeltaTime);

	void PressedSpecial() override;
};
