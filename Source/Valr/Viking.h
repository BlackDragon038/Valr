// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Viking.generated.h"

/**
 * 
 */
UCLASS()
class VALR_API AViking : public AFighterPawn
{
	GENERATED_BODY()
public:
	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float specialDrainSpeed = 0.5f;

	void PressedSpecial() override;
};
