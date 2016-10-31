// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BitBomb.generated.h"

UCLASS()
class BOMBIT_API ABitBomb : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = MeshSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BombMesh;
	
public:	
	// Sets default values for this actor's properties
	ABitBomb();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
