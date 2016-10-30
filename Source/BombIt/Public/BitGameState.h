// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "BitGameState.generated.h"

/**
 * 
 */
UCLASS()
class BOMBIT_API ABitGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABitGameState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Transient, Category = GameSettings)
	int32 MaxRoundTime;
	
	UPROPERTY(BlueprintReadWrite, Transient, Category = GameSettings)
	int32 RemainingTime;
	
};
