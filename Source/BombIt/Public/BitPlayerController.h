// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BOMBIT_API ABitPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	ABitPlayerController();
	
	void FlushDebugLines();

protected:	
	/** sets up input */
	virtual void SetupInputComponent() override;
};
