// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FightingGameMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GAME_STATE : uint8 { SINGLEPLAYER, MULTIPLAYER};

UCLASS()
class VALR_API AFightingGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		GAME_STATE GameMode = GAME_STATE::SINGLEPLAYER;
	
};
