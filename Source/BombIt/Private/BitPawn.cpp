// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitPawn.h"


// Sets default values
ABitPawn::ABitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRootComponent"));
	RootComponent = DummyRootComponent;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DummyRootComponent);
	Camera->bUsePawnControlRotation = true;

}


/* setup initial variables */
void ABitPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ABitPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABitPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ABitPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set up gameplay key bindings
	check(InputComponent);
			
	// Bind Actions
	InputComponent->BindAction("PlaceBomb", IE_Pressed, this, &ABitPawn::PlaceBomb);
	InputComponent->BindAction("DetonateBomb", IE_Pressed, this, &ABitPawn::DetonateBomb);

	// Bind Axis
	InputComponent->BindAxis("MoveForward", this, &ABitPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABitPawn::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ABitPawn::PlaceBomb()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, FString::Printf(TEXT("PlaceBomb!!!")));
}

void ABitPawn::DetonateBomb()
{
	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, FString::Printf(TEXT("DetonateBomb!!!")));
}

void ABitPawn::MoveForward(float Value)
{	
	if (Value != 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, FString::Printf(TEXT("MoveForward: %f"), Value));
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABitPawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABitPawn::LookAtTarget(const AActor &Target)
{
	
}