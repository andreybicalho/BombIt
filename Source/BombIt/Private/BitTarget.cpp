// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitTarget.h"


// Sets default values
ABitTarget::ABitTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));
	RootComponent = DestructibleComponent;

}

// Called when the game starts or when spawned
void ABitTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABitTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

