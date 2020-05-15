// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Janissary.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API AJanissary : public AFighterPawn
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	bool bBoosted = false;

	uint8 Damages[3] = {0};

	void BeginPlay() override;

	bool bSuperCharged = false;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float specialDrainSpeed = 0.5f;

	void PressedSpecial() override;
	
};
