// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera.h"
#include "FighterPawn.h"

#include "FightManager.generated.h"

UENUM(BlueprintType)
enum class ROUND_STATE : uint8 { ROUND_OVER_PLAYER1_WINS, ROUND_OVER_PLAYER2_WINS, ROUND_DRAW, ROUND_RESTARTING, ROUND_ABOUT_TO_START, ROUND_ONGOING, MATCH_OVER_PLAYER1_WINS, MATCH_OVER_PLAYER2_WINS };

UCLASS()
class VALR_API AFightManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFightManager();

	UPROPERTY(BlueprintReadWrite)
		UFightingGameInstance* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFighterPawn* Player1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFighterPawn* Player2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACamera* Camera = nullptr;

	UPROPERTY(BlueprintReadOnly)
		int32 roundTimer = 3600;

	UPROPERTY(BlueprintReadOnly)
		int32 timeToStart = 180;

	UPROPERTY(BlueprintReadOnly)
		uint8 roundCount = 1;

	UPROPERTY(BlueprintReadOnly)
		ROUND_STATE roundState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float defaultDistanceBetweenPlayers = 350;

	bool bCoinsIncreased = false;

	int trainingTimer = 0;

	uint8 randomAttackIndex = 0;
	uint8 randomMovementIndex = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void processPlayer(AFighterPawn* &P1, AFighterPawn* &P2, FVector toP1, FVector toP2);

	void testRoundStatus();

	void CheckTrainingMode();

	void calculateTimers(FVector toP1, FVector toP2);

public:	

	static float Angle(FVector a, FVector b);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
