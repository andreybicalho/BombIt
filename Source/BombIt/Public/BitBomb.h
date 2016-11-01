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
	
	/* 
	* Sound and FXs
	*/

	UPROPERTY(EditDefaultsOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ExplosionFX;

	/*
	* Shockwave
	*/

	UPROPERTY(Category = ShockwaveSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ShockwaveCollision;

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	float ShockwaveMaxRadius;

	float ShockwaveCurrentRadius;

	// delegates for OnBeginOverlap event
	FScriptDelegate OnBeginOverlapShockwaveDelegate;

	// callback function for the OnBeginOverlap
	void ShockwaveOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep);

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ShockwaveSpeedCurve;

	FTimeline ShockwaveTimeline;	
	
	UFUNCTION()
	void IncreaseShockwaveRadiusTimeLineCallback(float Value);
	
public:	
	// Sets default values for this actor's properties
	ABitBomb();

	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Explode();
	
};
