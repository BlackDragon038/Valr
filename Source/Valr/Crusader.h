// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Crusader.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ACrusader : public AFighterPawn
{
public:
	GENERATED_BODY()

	void BeginPlay();
	
	void Tick(float DeltaTime);

	void PressedSpecial() override;
	
};
