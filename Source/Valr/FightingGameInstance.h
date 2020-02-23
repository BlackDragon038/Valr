// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FightingGameInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GAME_STATE : uint8 { SINGLEPLAYER, MULTIPLAYER, TRAINING };

UENUM(BlueprintType)
enum class WARRIOR : uint8 { CRUSADER, VIKING, SAMURAI, KNIGHT, NINJA };

UCLASS()
class VALR_API UFightingGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		GAME_STATE GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		float maxDistanceFromCenter = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WARRIOR Player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WARRIOR Player2;


};
