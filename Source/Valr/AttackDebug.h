// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "FighterPawn.h"
#include "Components/ActorComponent.h"
#include "AttackDebug.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALR_API UAttackDebug : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackDebug();

	AFighterPawn *Fighter = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
