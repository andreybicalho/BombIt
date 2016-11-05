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


	ShockwaveMaxRadius = 500.f;
	ShockwaveCurrentRadius = 0.1f;

	ShockwaveCollisionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ShockwaveCollisionTrigger"));
	ShockwaveCollisionTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShockwaveCollisionTrigger->InitSphereRadius(ShockwaveCurrentRadius);
	// shockwave collision settings	
	ShockwaveCollisionTrigger->SetCollisionProfileName("ShockwaveCollisionProfileOverlap");
	ShockwaveCollisionTrigger->SetSimulatePhysics(false);
	ShockwaveCollisionTrigger->SetNotifyRigidBodyCollision(false); // we don't want "Simulation Generates Hit Events" on
	// binding OnBeginOverlap callback function
	ShockwaveCollisionTrigger->bGenerateOverlapEvents = true;
	OnBeginOverlapShockwaveDelegate.BindUFunction(this, "ShockwaveOverlap");
	ShockwaveCollisionTrigger->OnComponentBeginOverlap.Add(OnBeginOverlapShockwaveDelegate);

	bUseInstantShockwave = false;

	// since we have a profile we don't need to customize it like below:
	/*ShockwaveCollisionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShockwaveCollisionTrigger->SetCollisionObjectType(SHOCKWAVE);
	ShockwaveCollisionTrigger->SetCollisionResponseToAllChannels(ECR_Block);
	ShockwaveCollisionTrigger->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ShockwaveCollisionTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ShockwaveCollisionTrigger->SetCollisionResponseToChannel(SHOCKWAVE, ECR_Ignore);*/

	// shockwave range display
	ShockwaveRangeDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShockwaveRangeDisplay"));
	ShockwaveRangeDisplay->AttachToComponent(ShockwaveCollisionTrigger, FAttachmentTransformRules::KeepRelativeTransform);
	ShockwaveRangeDisplay->SetCollisionProfileName("ShockwaveRangeDisplay");
	ShockwaveRangeDisplay->SetSimulatePhysics(false);
	ShockwaveRangeDisplay->SetNotifyRigidBodyCollision(false);
	float ShockwaveDisplayInitialSize = ShockwaveCurrentRadius / 50.f;
	ShockwaveRangeDisplay->SetRelativeScale3D(FVector(ShockwaveDisplayInitialSize, ShockwaveDisplayInitialSize, ShockwaveDisplayInitialSize));
	
	//
	/*ShockwaveRangeDisplay->bGenerateOverlapEvents = true;
	OnBeginOverlapShockwaveRangeDisplayDelegate.BindUFunction(this, "ShockwaveRangeDisplayBeginOverlap");
	ShockwaveRangeDisplay->OnComponentBeginOverlap.Add(OnBeginOverlapShockwaveRangeDisplayDelegate);*/
	//
	OnEndOverlapShockwaveRangeDisplayDelegate.BindUFunction(this, "ShockwaveRangeDisplayEndOverlap");
	ShockwaveRangeDisplay->OnComponentEndOverlap.Add(OnEndOverlapShockwaveRangeDisplayDelegate);


	// Main Mesh
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	BombMesh->AttachToComponent(RootSceneComp, FAttachmentTransformRules::KeepRelativeTransform);
	BombMesh->SetCollisionProfileName("BombeMeshCollisionProfile");
	BombMesh->bGenerateOverlapEvents = true;
	//
	ShockwaveRangeDisplay->bGenerateOverlapEvents = true;
	OnBeginOverlapShockwaveRangeDisplayDelegate.BindUFunction(this, "ShockwaveRangeDisplayBeginOverlap");
	BombMesh->OnComponentBeginOverlap.Add(OnBeginOverlapShockwaveRangeDisplayDelegate);

	bIsActivated = true;

	// radial force
	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->AttachToComponent(RootSceneComp, FAttachmentTransformRules::KeepRelativeTransform);
	RadialForce->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialForce->Radius = ShockwaveMaxRadius;
	// RadialForce->ObjectTypesToAffect.RemoveAll();
	// RadialForce->ObjectTypesToAffect.Add(EObjectTypeQuery)
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
		InterpFloatFunction.BindUFunction(this, "ShockwaveTick");

		ShockwaveTimeline = FTimeline();
		ShockwaveTimeline.AddInterpFloat(ShockwaveSpeedCurve, InterpFloatFunction, "InterpFloatFunction");

		FOnTimelineEventStatic TimelineFinishedFunction = FOnTimelineEventStatic();
		TimelineFinishedFunction.BindUFunction(this, "ShockwaveFinished");
		ShockwaveTimeline.SetTimelineFinishedFunc(TimelineFinishedFunction);
	}
	else
	{
		bUseInstantShockwave = true;
	}


	// shockwave display timeline
	if (ShockwaveDisplaySpeedCurve)
	{
		//Setup the timelinefunction
		FOnTimelineFloat InterpFloatFunction = FOnTimelineFloat();
		InterpFloatFunction.BindUFunction(this, "ShockwaveDisplayTick");

		ShockwaveDisplayTimeline = FTimeline();
		ShockwaveDisplayTimeline.AddInterpFloat(ShockwaveDisplaySpeedCurve, InterpFloatFunction, "InterpFloatFunction");
	}
}

// Called when the game starts or when spawned
void ABitBomb::BeginPlay()
{
	Super::BeginPlay();

	ShockwaveDisplayTimeline.PlayFromStart();
		
}

// Called every frame
void ABitBomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// tick shockwave timeline
	if (ShockwaveTimeline.IsPlaying())
	{
		ShockwaveTimeline.TickTimeline(DeltaTime);
	}

	// tick shockwave display timeline
	if (ShockwaveDisplayTimeline.IsPlaying())
	{
		ShockwaveDisplayTimeline.TickTimeline(DeltaTime);
	}

}

void ABitBomb::Explode()
{
	if (!bIsActivated)
	{
		return;
	}
	
	bIsActivated = false;
	
	//
	ShockwaveRangeDisplay->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShockwaveRangeDisplay->SetHiddenInGame(true);
	ShockwaveRangeDisplay->Activate(false);
	//
	BombMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BombMesh->SetHiddenInGame(true);
	BombMesh->Activate(false);
	
	
	// play sound
	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAttached(ExplosionSound, this->GetRootComponent());
	}

	// fx
	if (ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), GetActorRotation());
	}

	// start shockwave
	if (!bUseInstantShockwave)
	{
		ShockwaveTimeline.PlayFromStart();
	}
	else
	{
		ShockwaveCollisionTrigger->SetSphereRadius(ShockwaveMaxRadius, true);
		ShockwaveFinished();
	}
}

void ABitBomb::ShockwaveOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep)
{
	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow, FString::Printf(TEXT("ShockwaveOverlap")));

	ABitBomb* OtherBomb = Cast<ABitBomb>(OtherComp);
	if (OtherBomb && OtherBomb != this && OtherBomb->isActivated())
	{
		OtherBomb->Explode();
	}
}

void ABitBomb::ShockwaveTick(float Value)
{	
	float ShockwaveRadius = Value * ShockwaveMaxRadius;

	ShockwaveCollisionTrigger->SetSphereRadius(ShockwaveRadius, true);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shockwave: timeline: %f  value: %f  ShockwaveRadius: %f"), ShockwaveTimeline.GetPlaybackPosition(), Value, ShockwaveRadius));
}

void ABitBomb::ShockwaveFinished()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("ShockwaveFinished")));
	
	ShockwaveCollisionTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShockwaveCollisionTrigger->Activate(false);	

	// apply radial force
	RadialForce->FireImpulse();
	
	Destroy();
}

bool ABitBomb::isActivated()
{
	return bIsActivated;
}

void ABitBomb::ShockwaveRangeDisplayBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep)
{	
	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, FString::Printf(TEXT("ShockwaveRangeDisplayBeginOverlap")));

	ABitBomb* OtherBomb = Cast<ABitBomb>(OtherComp);
	if (OtherBomb && OtherBomb != this && OtherBomb->isActivated())
	{
		OtherBomb->BombInRange();

		/*if (ShockwaveCollidedMaterial)
		{
			ShockwaveRangeDisplay->SetMaterial(0, ShockwaveCollidedMaterial);
		}*/
	}	
}

void ABitBomb::ShockwaveRangeDisplayEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, FString::Printf(TEXT("ShockwaveRangeDisplayEndOverlap")));
	
	if (bIsActivated) 
	{
		ABitBomb* OtherBomb = Cast<ABitBomb>(OtherComp);
		if (OtherBomb && OtherBomb != this && OtherBomb->isActivated())
		{
			TArray<AActor*> OverlapingBombs;
			ShockwaveRangeDisplay->GetOverlappingActors(OverlapingBombs, ABitBomb::StaticClass());

			if (OverlapingBombs.Num() <= 1 && ShockwaveCollidedMaterial)
			{
				ShockwaveRangeDisplay->SetMaterial(0, ShockwaveNotCollidedMaterial);
			}
		}
	}


}

void ABitBomb::ShockwaveDisplayTick(float Value)
{
	float ShockwaveDisplayRadius = (Value * ShockwaveMaxRadius) / 50.f;

	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, FString::Printf(TEXT("ShockwaveDisplayTick, Radius: %f"), ShockwaveDisplayRadius));
	
	ShockwaveRangeDisplay->SetRelativeScale3D(FVector(ShockwaveDisplayRadius, ShockwaveDisplayRadius, ShockwaveDisplayRadius));
}

void ABitBomb::BombInRange()
{
	if (ShockwaveCollidedMaterial)
	{
		ShockwaveRangeDisplay->SetMaterial(0, ShockwaveCollidedMaterial);
	}
}

void ABitBomb::SetShockwaveRadius(float Radius)
{
	float NewShockwaveDisplayRadius = (Radius / 50.f) + ShockwaveRangeDisplay->RelativeScale3D.X;

	ShockwaveRangeDisplay->SetRelativeScale3D(FVector(NewShockwaveDisplayRadius, NewShockwaveDisplayRadius, NewShockwaveDisplayRadius));

	float NewShockwaveRadius = Radius + ShockwaveCollisionTrigger->GetScaledSphereRadius();

	ShockwaveMaxRadius = ShockwaveMaxRadius + Radius;
	
	ShockwaveCollisionTrigger->SetSphereRadius(NewShockwaveRadius, true);	
}



