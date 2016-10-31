// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BitTarget.generated.h"

UCLASS()
class BOMBIT_API ABitTarget : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Category = DestructibleSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* DestructibleComponent;

	
	
public:	
	// Sets default values for this actor's properties
	ABitTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
