// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FightingGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FColorData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float R = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float G = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float B = 0;
};

USTRUCT(BlueprintType)
struct FCharacterColorData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColorData Headgear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColorData Armor_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColorData Armor_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColorData Special_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColorData Special_2;
};

UENUM(BlueprintType)
enum class GAME_STATE : uint8 { SINGLEPLAYER_STORY_MODE, SINGLEPLAYER_SINGLE_MATCH, MULTIPLAYER, TRAINING };

UENUM(BlueprintType)
enum class WARRIOR : uint8 { CRUSADER, VIKING, SAMURAI, KNIGHT, NINJA, SPARTAN, SOLDIER, JANISSARY};

UENUM(BlueprintType)
enum class DIFFICULTY : uint8 { EASY, NORMAL, HARD };

UCLASS()
class VALR_API UFightingGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UFightingGameInstance();

	void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Save Player File")
		void SavePlayerFile();
	
	UFUNCTION(BlueprintCallable, Category = "Load Player File")
		void LoadPlayerFile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		GAME_STATE GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		DIFFICULTY Difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		float maxDistanceFromCenter = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WARRIOR Player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		WARRIOR Player2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int PlayerCoins = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Crusader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Viking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Samurai;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Knight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Ninja;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Spartan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Soldier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterColorData Janissary;
};
