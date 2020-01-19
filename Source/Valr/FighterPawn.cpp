// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"

// Sets default values
AFighterPawn::AFighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFighterPawn::BeginPlay()
{
	Super::BeginPlay();

	{ //Setting up partial sums so they won't have to be calculated later
		int counter = 0;
		for (auto it : attacks)
			for (auto it2 : it.frames) {
				counter += it2.hold;
				it2.psum = counter;
			}
	}
}

// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("W", IE_Pressed, this, &AFighterPawn::PressedW);
	PlayerInputComponent->BindAction("A", IE_Pressed, this, &AFighterPawn::PressedA);
	PlayerInputComponent->BindAction("S", IE_Pressed, this, &AFighterPawn::PressedS);
	PlayerInputComponent->BindAction("D", IE_Pressed, this, &AFighterPawn::PressedD);
	PlayerInputComponent->BindAction("W", IE_Released,this, &AFighterPawn::ReleasedW);
	PlayerInputComponent->BindAction("A", IE_Released,this, &AFighterPawn::ReleasedA);
	PlayerInputComponent->BindAction("S", IE_Released,this, &AFighterPawn::ReleasedS);
	PlayerInputComponent->BindAction("D", IE_Released,this, &AFighterPawn::ReleasedD);
	PlayerInputComponent->BindAction("1", IE_Pressed,  this, &AFighterPawn::Pressed1);
	PlayerInputComponent->BindAction("2", IE_Pressed,  this, &AFighterPawn::Pressed2);
	PlayerInputComponent->BindAction("3", IE_Pressed,  this, &AFighterPawn::Pressed3);
	PlayerInputComponent->BindAction("4", IE_Pressed,  this, &AFighterPawn::Pressed4);
	/*PlayerInputComponent->BindAction("5", IE_Pressed, &AFighterPawn::Pressed5);
	PlayerInputComponent->BindAction("6", IE_Pressed, &AFighterPawn::Pressed6);
	PlayerInputComponent->BindAction("7", IE_Pressed, &AFighterPawn::Pressed7);
	PlayerInputComponent->BindAction("8", IE_Pressed, &AFighterPawn::Pressed8);
	PlayerInputComponent->BindAction("9", IE_Pressed, &AFighterPawn::Pressed9);
	PlayerInputComponent->BindAction("10", IE_Pressed, &AFighterPawn::Pressed10);
	PlayerInputComponent->BindAction("11", IE_Pressed, &AFighterPawn::Pressed11);
	PlayerInputComponent->BindAction("12", IE_Pressed, &AFighterPawn::Pressed12);*/
}

//-   0
//W   1
//A   2
//S   3
//D   4
//WA  5
//WD  6
//SA  7
//SD  8
//... 9+

void AFighterPawn::PressedW()  {
	if (lock_frames != 0) return;
	if (i == 2) { i = 5; return; }
	if (i == 4) { i = 6; return; }
	{ i = 1; return; }
}
void AFighterPawn::PressedA()  {
	if (lock_frames != 0) return;
	if (i == 1) { i = 5; return; }
	if (i == 3) { i = 7; return; }
	{ i = 2; return; }
}
void AFighterPawn::PressedS()  {
	if (lock_frames != 0) return;
	if (i == 2) { i = 7; return; }
	if (i == 4) { i = 8; return; }
	{ i = 3; return; }
}
void AFighterPawn::PressedD() {
	if (lock_frames != 0) return;
	if (i == 1) { i = 6; return; }
	if (i == 3) { i = 8; return; }
	{ i = 4; return; }
}
void AFighterPawn::ReleasedW() {
	if (lock_frames != 0) return;
	if (i == 5) { i = 2; return; }
	if (i == 6) { i = 4; return; }
	{ i = 0; return; }
}
void AFighterPawn::ReleasedA() {
	if (lock_frames != 0) return;
	if (i == 5) { i = 1; return; }
	if (i == 7) { i = 3; return; }
	{ i = 0; return; }
}
void AFighterPawn::ReleasedS() {
	if (lock_frames != 0) return;
	if (i == 7) { i = 2; return; }
	if (i == 8) { i = 4; return; }
	{ i = 0; return; }
}
void AFighterPawn::ReleasedD() {
	if (lock_frames != 0) return;
	if (i == 6) { i = 1; return; }
	if (i == 8) { i = 3; return; }
	{ i = 0; return; }
}

void AFighterPawn::Pressed1() {
	if (lock_frames == 0) { i = 9; lock_frames = attacks[0].frames[attacks[0].frames.Num()-1].psum; }
}

void AFighterPawn::Pressed2() {
	if (lock_frames == 0) { i = 10; lock_frames = attacks[1].frames[attacks[1].frames.Num()-1].psum; }
}

void AFighterPawn::Pressed3() {
	if (lock_frames == 0) { i = 11; lock_frames = attacks[2].frames[attacks[2].frames.Num()-1].psum; }
}

void AFighterPawn::Pressed4() {
	if (lock_frames == 0) { i = 12; lock_frames = attacks[3].frames[attacks[3].frames.Num()-1].psum; }
}