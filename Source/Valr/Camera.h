// Code by Sultan Iljasov, 2020.

#pragma once

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "FighterPawn.h"
#include "Camera/CameraShake.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class VALR_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float closestDistance = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Height = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float initialCameraDistance = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShake> CameraShake;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
