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
	class USphereComponent* ShockwaveCollisionTrigger;

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	float ShockwaveMaxRadius;

	float ShockwaveCurrentRadius;

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ShockwaveSpeedCurve;

	FTimeline ShockwaveTimeline;	
	
	// callback function for the FTimeline tick
	UFUNCTION()
	void ShockwaveTick(float Value);

	UFUNCTION()
	void ShockwaveTimelineFinished();

	// whether the bomb can explode
	bool bIsActivated;

	/*
	* Shockwave range display overlap
	*/

	UPROPERTY(Category = ShockwaveRangeDisplaySettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShockwaveRangeDisplay;

	// delegates for OnBeginOverlap and OnEndOverlap events for the ShockwaveRangeDisplay
	FScriptDelegate OnBeginOverlapShockwaveRangeDisplayDelegate;
	FScriptDelegate OnEndOverlapShockwaveRangeDisplayDelegate;

	UFUNCTION()
	void ShockwaveRangeDisplayBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep);

	UFUNCTION()
	void ShockwaveRangeDisplayEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Category = ShockwaveRangeDisplaySettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ShockwaveCollidedMaterial;

	UPROPERTY(Category = ShockwaveRangeDisplaySettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ShockwaveNotCollidedMaterial;


	UPROPERTY(EditDefaultsOnly, Category = ShockwaveRangeDisplaySettings, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ShockwaveDisplaySpeedCurve;

	FTimeline ShockwaveDisplayTimeline;

	UFUNCTION()
	void ShockwaveDisplayTick(float Value);
	

	/*
	* BombMesh Overlap
	*/

	UPROPERTY(Category = BombMeshSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* BombMeshCollidedMaterial;

	UPROPERTY(Category = BombMeshSettings, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* BombMesNotCollidedMaterial;

	FScriptDelegate OnBeginOverlapBombMeshDelegate;
	FScriptDelegate OnEndOverlapBombMeshDelegate;

	UFUNCTION()
	void BombMeshOnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep);

	UFUNCTION()
	void BombMeshOnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	/*
	*	Radial Force
	*/

	UPROPERTY(EditDefaultsOnly, Category = ShockwaveSettings, meta = (AllowPrivateAccess = "true"))
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

	/*
	* Dev Helpers
	*/

	void SetShockwaveRadius(float Radius);
	
};
