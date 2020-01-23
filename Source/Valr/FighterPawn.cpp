// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"


// Sets default values
AFighterPawn::AFighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFighterPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InputID == INPUT::W) { SetActorRelativeLocation({ 0,10,0 }); UE_LOG(LogTemp, Warning, TEXT("W")) }
	if (InputID == INPUT::A) { SetActorRelativeLocation({ -10,0,0 }); UE_LOG(LogTemp, Warning, TEXT("A")) }
	if (InputID == INPUT::S) { SetActorRelativeLocation({ 0,-10,0 }); UE_LOG(LogTemp, Warning, TEXT("S")) }
	if (InputID == INPUT::D) { SetActorRelativeLocation({ 10,0,0 }); UE_LOG(LogTemp, Warning, TEXT("D")) }

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
	PlayerInputComponent->BindAction("Light", IE_Pressed,  this, &AFighterPawn::PressedLight);
	PlayerInputComponent->BindAction("Medium", IE_Pressed,  this, &AFighterPawn::PressedMedium);
	PlayerInputComponent->BindAction("Heavy", IE_Pressed,  this, &AFighterPawn::PressedHeavy);
	PlayerInputComponent->BindAction("Special", IE_Pressed,  this, &AFighterPawn::PressedSpecial);
}

void AFighterPawn::PressedW()  
{
	InputID = INPUT::W;
	UE_LOG(LogTemp, Warning, TEXT("Moving UP"));
}
void AFighterPawn::PressedA()  
{
	InputID = INPUT::A;
	UE_LOG(LogTemp, Warning, TEXT("Moving LEFT"));
}
void AFighterPawn::PressedS()  
{
	InputID = INPUT::S;
	UE_LOG(LogTemp, Warning, TEXT("Moving DOWN"));
}
void AFighterPawn::PressedD() 
{
	InputID = INPUT::D;
	UE_LOG(LogTemp, Warning, TEXT("Moving RIGHT"));
}
void AFighterPawn::ReleasedW() 
{
	InputID = INPUT::Idle;
	UE_LOG(LogTemp, Warning, TEXT("Moving Stopped"));
}
void AFighterPawn::ReleasedA() 
{
	InputID = INPUT::Idle;
	UE_LOG(LogTemp, Warning, TEXT("Moving Stopped"));
}
void AFighterPawn::ReleasedS() 
{
	InputID = INPUT::Idle;
	UE_LOG(LogTemp, Warning, TEXT("Moving Stopped"));
}
void AFighterPawn::ReleasedD() 
{
	InputID = INPUT::Idle;
	UE_LOG(LogTemp, Warning, TEXT("Moving Stopped"));
}

void AFighterPawn::PressedLight() 
{
	UE_LOG(LogTemp, Warning, TEXT("Light Attack"));
}

void AFighterPawn::PressedMedium() 
{
	UE_LOG(LogTemp, Warning, TEXT("Medium Attack"));
}

void AFighterPawn::PressedHeavy() 
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));
}

void AFighterPawn::PressedSpecial() 
{
	UE_LOG(LogTemp, Warning, TEXT("Special Attack"));
}