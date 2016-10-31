// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitBomb.h"


// Sets default values
ABitBomb::ABitBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

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

}

