// Code by Sultan Iljasov, 2020.

#pragma once

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "FightingGameInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <vector>

#include "FighterPawn.generated.h"

class AFightManager;

USTRUCT(BlueprintType)
struct FBlockData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 staminaCost = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Cooldown = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 blockStunRate = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 blockPushPower = 1;
};
 
USTRUCT(BlueprintType)
struct FFrameData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Hold = 1;
		uint8 PSum = 0;
};

USTRUCT(BlueprintType)
struct FAttackData 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		uint8 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 staminaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 StunRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 StunPushPower = 5;
	UPROPERTY(EditAnywhere)
		bool bUnblockableAttack = false;
	UPROPERTY(EditAnywhere)
		float attackPush = 0.f;
	UPROPERTY(EditAnywhere)
		TArray<FFrameData> Parts;
	UPROPERTY(BlueprintReadOnly)
	int AttackTotalFrameCount = 0;
};

UENUM(BlueprintType)
enum class INPUT : uint8 { IDLE, UP, UP_LEFT, LEFT, LEFT_DOWN, DOWN, DOWN_RIGHT, RIGHT, RIGHT_UP, LIGHT, MEDIUM, HEAVY, SPECIAL, BLOCK };

UENUM(BlueprintType)
enum class ATTACK_TYPE : uint8 { LIGHT, MEDIUM, HEAVY, SPECIAL, NONE };

UENUM(BlueprintType)
enum class STATE : uint8 { IDLE, MOVING, BLOCKING, ATTACKING, STUNNED, STEPPING, KNOCKED_DOWN, GETTING_UP};

UCLASS()
class VALR_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	enum class KEY_STATE { RESET, RELEASED, PRESSED_ONCE, PRESSED_TWICE };
	// Sets default values for this pawn's properties
	AFighterPawn();

	UFightingGameInstance* Instance;

	AFightManager *Manager;

	UPROPERTY(BlueprintReadOnly)
		FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* hitParticle;

	UPROPERTY(BlueprintReadOnly)
		INPUT InputID = INPUT::IDLE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 Lives = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 Health = 255;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 inputBufferSize = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		uint8 MovementSpeed = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		uint8 sideStepSpeed = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		uint8 sideStepFrameTime = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		uint8 turnSpeed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 Stamina = 100;
	UPROPERTY(BlueprintReadOnly)
		uint8 specialMeter = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
		float specialChargeMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		uint8 staminaRegeneration = 2;
	UPROPERTY(BlueprintReadOnly)
		STATE State = STATE::IDLE;

	UPROPERTY(BlueprintReadOnly)
	uint8 currentFrameOfAttack = 0;

	UPROPERTY(BlueprintReadOnly)
		uint8 blockCooldown = 0;

	uint8 currentPartsIndex = 0;
	uint8 stunPush = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data")
		TArray<FAttackData> Attacks;

	std::vector<INPUT> inputBuffer;
	uint8 inputBufferIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block Data")
		FBlockData BlockData;

	UPROPERTY(BlueprintReadOnly)
		ATTACK_TYPE attackType = ATTACK_TYPE::NONE;

	uint8 steppingSpeed = 0;
	uint8 steppingFrameTime = 0;
	bool bDoubleTapW = false;
	bool bDoubleTapA = false;
	bool bDoubleTapS = false;
	bool bDoubleTapD = false;
	KEY_STATE KeyW = KEY_STATE::RESET;
	KEY_STATE KeyA = KEY_STATE::RESET;
	KEY_STATE KeyS = KEY_STATE::RESET;
	KEY_STATE KeyD = KEY_STATE::RESET;

	UPROPERTY(BlueprintReadOnly)
	bool UP_Key = 0;
	UPROPERTY(BlueprintReadOnly)
	bool LEFT_Key = 0;
	UPROPERTY(BlueprintReadOnly)
	bool DOWN_Key = 0;
	UPROPERTY(BlueprintReadOnly)
	bool RIGHT_Key = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bResetAnimation = false;

	UPROPERTY(BlueprintReadOnly)
	bool bOpponentIsHit = false;

	UPROPERTY(BlueprintReadOnly)
	bool bNonStunnable = false;

	UPROPERTY(BlueprintReadOnly)
	bool bDisableInput = false;

	bool bCanCombo = true;

	bool bAllowParry = false;

	//Training Variables
	UPROPERTY(BlueprintReadWrite)
		bool bRandomMovement = false;

	UPROPERTY(BlueprintReadWrite)
		bool bRandomAttacks = false;

	UPROPERTY(BlueprintReadWrite)
		bool bRandomBlocking = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	void Reset();

	void AttackReset();

	void testSideStep();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void WalkBackToPosition(FVector Pos);
	void AxisW(float Axis);
	void AxisA(float Axis);
	void AxisS(float Axis);
	void AxisD(float Axis);
	void PressedW();
	void PressedA();
	void PressedS();
	void PressedD();
	void ReleasedW();
	void ReleasedA();
	void ReleasedS();
	void ReleasedD();
	void PressedLight();
	void PressedMedium();
	void PressedHeavy();
	virtual void PressedSpecial();
	virtual void ReleasedSpecial(){}
	void PressedBlock();
	void ReleasedBlock();
	void AxisBlock(float Axis);
};
