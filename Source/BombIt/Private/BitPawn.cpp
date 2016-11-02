// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitPawn.h"
#include "BitPlayerController.h"
#include "BitBomb.h"


// Sets default values
ABitPawn::ABitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = false; // since we are rotating our actor by ourselves


	// default bomb to spawn
	static ConstructorHelpers::FClassFinder<ABitBomb> DefaultBombToSpawnBP(TEXT("/Game/Blueprints/Bombs/Bomb_BP"));
	if (DefaultBombToSpawnBP.Class != NULL)
	{
		DefaultBombToSpawn = DefaultBombToSpawnBP.Class;
	}

	RotationSense = 1.f;
	MoveSense = 1.f;


	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Game/Materials/M_Cursor_Decal"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
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

	if (CursorToWorld != nullptr)
	{
		if (ABitPlayerController* PC = Cast<ABitPlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

// Called to bind functionality to input
void ABitPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set up gameplay key bindings
	check(InputComponent);	

	// Bind Actions
	InputComponent->BindAction("PlaceBomb", IE_Released, this, &ABitPawn::PlaceBomb);
	InputComponent->BindAction("DetonateBomb", IE_Released, this, &ABitPawn::DetonateBomb);
	InputComponent->BindAction("SelectBomb", IE_Pressed, this, &ABitPawn::SelectBomb);

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

	ABitPlayerController* MyPC = Cast<ABitPlayerController>(GetController());
	if (MyPC)
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		MyPC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, spawn bomb there
			if (DefaultBombToSpawn)
			{
				CurrentSelectedBomb = GetWorld()->SpawnActor<ABitBomb>(DefaultBombToSpawn, Hit.ImpactPoint, FRotator::ZeroRotator);
				// TODO: attach bomb there
			}

		}
	}
	
}

void ABitPawn::DetonateBomb()
{
	if (CurrentSelectedBomb)
	{
		CurrentSelectedBomb->Explode();
	}
}

void ABitPawn::SelectBomb()
{
	ABitPlayerController* MyPC = Cast<ABitPlayerController>(GetController());
	if (MyPC)
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		MyPC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{			
			GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, FString::Printf(TEXT("Found something!!!")));
		}
	}
}

void ABitPawn::MoveForward(float Value)
{	
	if (Value != 0.0f)
	{		
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * Value * MoveSense; // FMath::Clamp<float>(Value, -1.0f, 1.0f);
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