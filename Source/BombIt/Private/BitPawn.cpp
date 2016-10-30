// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitPawn.h"


// Sets default values
ABitPawn::ABitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = false; // since we are rotating our actor by ourselves

	RotationSense = 1.f;
	MoveSense = 1.f;
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
//void ABitPawn::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//}

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
	InputComponent->BindAxis("MoveUp", this, &ABitPawn::MoveUp);
	InputComponent->BindAxis("CameraPitch", this, &ABitPawn::CameraPitch);
	InputComponent->BindAxis("CameraYaw", this, &ABitPawn::CameraYaw);
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
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * Value * MoveSense; // FMath::Clamp<float>(Value, -1.0f, 1.0f);;
		SetActorLocation(NewLocation);
	}
}

void ABitPawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorRightVector() * Value * MoveSense; // FMath::Clamp<float>(Value, -1.0f, 1.0f);
		SetActorLocation(NewLocation);
	}
}

void ABitPawn::MoveUp(float Value)
{
	if (Value != 0.0f)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorUpVector() * Value * MoveSense; // FMath::Clamp<float>(Value, -1.0f, 1.0f);
		SetActorLocation(NewLocation);
	}
}

void ABitPawn::CameraPitch(float Value)
{
	if (Value != 0.0f)
	{		
		FRotator NewRotation = GetActorRotation();
		NewRotation.Pitch += Value * RotationSense;
		SetActorRotation(NewRotation);
	}
}

void ABitPawn::CameraYaw(float Value)
{
	if (Value != 0.0f)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += Value * RotationSense; // FMath::Clamp<float>(Value, -1.0f, 1.0f) * ControlSense;
		SetActorRotation(NewRotation);
	}
}

void ABitPawn::LookAtTarget(const AActor &Target)
{
	
}