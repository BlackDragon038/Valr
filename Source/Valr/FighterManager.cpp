// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterManager.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFighterManager::AFighterManager() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFighterManager::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AFighterManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (p1->i > 8) {
		int x = 0;
		for (auto it : p1->attacks[p1->i - 9].frames) {
			if ((p1->attacks[p1->i - 9].frames[p1->attacks[p1->i - 9].frames.Num() - 1].psum - p1->lock_frames) > it.psum) break; else { x++; };
		}
		if (p1->GetDistanceTo(p2) > p1->attacks[p1->i - 9].frames[x].min_dist 
			&& p1->GetDistanceTo(p2) < p1->attacks[p1->i - 9].frames[x].max_dist
			&& UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::InverseTransformDirection(p1->GetTransform(), p2->GetActorLocation())).Yaw > p1->attacks[p1->i - 9].frames[x].min_angle
		    && UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::InverseTransformDirection(p1->GetTransform(), p2->GetActorLocation())).Yaw < p1->attacks[p1->i - 9].frames[x].max_angle) {
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("Nah!"));

		/*p1->attacks[p1->i - 9].frames[x].min_angle;
		p1->attacks[p1->i - 9].frames[x].max_angle;


		p1->GetActorRotation().Yaw;*/
	}

	if (p2->i > 8) {

	}
}

