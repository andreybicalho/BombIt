// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BitPawn.generated.h"

UCLASS()
class BOMBIT_API ABitPawn : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(Category = PlayerRepresentation, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

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
	*	Inputs
	*/

	// actions
	void PlaceBomb();
	void DetonateBomb();

	// axis
	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	void LookAtTarget(const AActor &Target);
};
