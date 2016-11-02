// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitBomb.h"


// Sets default values
ABitBomb::ABitBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	RootComponent = RootSceneComp;

	ShockwaveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShockwaveCollision"));
	ShockwaveCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShockwaveCollision->InitSphereRadius(0.5f);
	// shockwave collision settings	
	ShockwaveCollision->SetCollisionProfileName("ShockwaveCollisionProfileOverlap");
	ShockwaveCollision->SetSimulatePhysics(false);
	ShockwaveCollision->SetNotifyRigidBodyCollision(false); // we don't want "Simulation Generates Hit Events" on
	// binding OnBeginOverlap callback function
	ShockwaveCollision->bGenerateOverlapEvents = true;
	OnBeginOverlapShockwaveDelegate.BindUFunction(this, "ShockwaveOverlap");
	ShockwaveCollision->OnComponentBeginOverlap.Add(OnBeginOverlapShockwaveDelegate);

	// since we have a profile we don't need to customize it like below:
	/*ShockwaveCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShockwaveCollision->SetCollisionObjectType(SHOCKWAVE);
	ShockwaveCollision->SetCollisionResponseToAllChannels(ECR_Block);
	ShockwaveCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ShockwaveCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ShockwaveCollision->SetCollisionResponseToChannel(SHOCKWAVE, ECR_Ignore);*/

	ShockwaveMaxRadius = 500.f;
	ShockwaveCurrentRadius = 0.5f;

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	BombMesh->AttachToComponent(ShockwaveCollision, FAttachmentTransformRules::KeepRelativeTransform);

	bIsActivated = false;
}

/* setup initial variables */
void ABitBomb::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// create the shockwave timeline
	if (ShockwaveSpeedCurve)
	{
		//Setup the timelinefunction
		FOnTimelineFloat InterpFloatFunction = FOnTimelineFloat();
		InterpFloatFunction.BindUFunction(this, "IncreaseShockwaveRadiusTimeLineCallback");

		ShockwaveTimeline = FTimeline();
		ShockwaveTimeline.AddInterpFloat(ShockwaveSpeedCurve, InterpFloatFunction, "InterpFloatFunction");

		FOnTimelineEventStatic TimelineFinishedFunction = FOnTimelineEventStatic();
		TimelineFinishedFunction.BindUFunction(this, "ShockwaveIncreasingFinished");
		ShockwaveTimeline.SetTimelineFinishedFunc(TimelineFinishedFunction);
	}
}

// Called when the game starts or when spawned
void ABitBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABitBomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (ShockwaveTimeline.IsPlaying())
	{
		ShockwaveTimeline.TickTimeline(DeltaTime);
	}

}

void ABitBomb::Explode()
{
	if (bIsActivated)
	{
		return;
	}
	
	bIsActivated = true;

	// shockwave
	ShockwaveTimeline.PlayFromStart();

	
	//UAudioComponent* AC = NULL;
	// play sound
	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAttached(ExplosionSound, this->GetRootComponent());
	}
}

void ABitBomb::ShockwaveOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep)
{
	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, FString::Printf(TEXT("ShockwaveOverlap")));
}

void ABitBomb::IncreaseShockwaveRadiusTimeLineCallback(float Value)
{	
	float ShockwaveRadius = Value * ShockwaveMaxRadius;

	ShockwaveCollision->SetSphereRadius(ShockwaveRadius, true);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shockwave: timeline: %f  value: %f  ShockwaveRadius: %f"), ShockwaveTimeline.GetPlaybackPosition(), Value, ShockwaveRadius));
}

void ABitBomb::ShockwaveIncreasingFinished()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Shockwave Increasing Finished!")));
	
	ShockwaveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Destroy();
}

