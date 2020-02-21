// Code by Sultan Iljasov, 2020.


#include "AttackDebug.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UAttackDebug::UAttackDebug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackDebug::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Fighter = Cast<AFighterPawn>(GetOwner());
}


// Called every frame
void UAttackDebug::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Fighter->State == STATE::ATTACKING)
	{
		FVector startMinAngle = Fighter->GetActorLocation() + Fighter->GetActorRightVector();
		FVector endMinAngle = Fighter->GetActorLocation() + Fighter->GetActorRightVector();
		startMinAngle.X = std::cos(FMath::DegreesToRadians(Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].minAngle)) * Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].minDist;
		endMinAngle.Y = std::sin(FMath::DegreesToRadians(Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].minAngle)) * Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].maxDist;
		
		FVector startMaxAngle = Fighter->GetActorLocation() + Fighter->GetActorRightVector();
		FVector endMaxAngle = Fighter->GetActorLocation() + Fighter->GetActorRightVector();
		startMaxAngle.X = std::cos(FMath::DegreesToRadians(Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].maxAngle)) * Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].minDist;
		endMaxAngle.Y = std::sin(FMath::DegreesToRadians(Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].maxAngle)) * Fighter->Attacks[static_cast<uint8>(Fighter->attackType)].Parts[Fighter->currentPartsIndex].maxDist;

		DrawDebugLine
		(
			GetWorld(),
			startMinAngle,
			endMinAngle,
			FColor::Red,
			false,
			1,
			1,
			1);
		DrawDebugLine
		(
			GetWorld(),
			startMaxAngle,
			endMaxAngle,
			FColor::Red,
			false,
			1,
			1,
			1);
	}
}

