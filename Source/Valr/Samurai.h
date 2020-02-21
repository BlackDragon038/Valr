// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Samurai.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API ASamurai : public AFighterPawn
{
	GENERATED_BODY()
public:

	void BeginPlay() override;

	void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float specialAttackSpeedMultiplier = 1.5f;

	void PressedSpecial() override;	
};
