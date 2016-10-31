// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BitPawn.generated.h"

UCLASS()
class BOMBIT_API ABitPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float RotationSense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float MoveSense;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/** default bomb to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BombSettings)
	TSubclassOf<class ABitBomb>  DefaultBombToSpawn;

	class ABitBomb* CurrentSelectedBomb;

public:
	// Sets default values for this pawn's properties
	ABitPawn();

	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/*
	*	Inputs and Movement
	*/

	// actions
	void PlaceBomb();
	void DetonateBomb();
	void SelectBomb();

	// axis
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void CameraPitch(float Value);
	void CameraYaw(float Value);

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	void LookAtTarget(const AActor &Target);
};
