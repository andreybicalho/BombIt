// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BitBomb.generated.h"

UCLASS()
class BOMBIT_API ABitBomb : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Category = RootSceneCompSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneComp;

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

	// if this is true shockwave will not be driven by the FTimeline
	UPROPERTY(Category = ShockwaveSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bUseInstantShockwave;

	UPROPERTY(Category = ShockwaveSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ShockwaveCollision;

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	float ShockwaveMaxRadius;

	float ShockwaveCurrentRadius;

	// delegates for OnBeginOverlap event
	FScriptDelegate OnBeginOverlapShockwaveDelegate;

	// callback function for the OnBeginOverlap
	UFUNCTION()
	void ShockwaveOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep);

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ShockwaveSpeedCurve;

	FTimeline ShockwaveTimeline;	
	
	// callback function for the FTimeline tick
	UFUNCTION()
	void ShockwaveTick(float Value);

	UFUNCTION()
	void ShockwaveFinished();

	// whether the bomb can explode
	bool bIsActivated;

	/* 
	*	Radial Force
	*/

	UPROPERTY(EditDefaultsOnly, Category = RadialForceSettings, meta = (AllowPrivateAccess = "true"))
	class URadialForceComponent* RadialForce;
	
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

	bool isActivated();
	
};
