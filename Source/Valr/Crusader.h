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

	void ReleasedSpecial() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 healEveryXFrame = 1;

	uint8 healEveryFrame = 0;
	
};
