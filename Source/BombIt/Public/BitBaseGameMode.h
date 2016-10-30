// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "BitBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BOMBIT_API ABitBaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABitBaseGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PreInitializeComponents() override;

	/** initialize replicated game data */
	virtual void InitGameState() override;

	/** Initialize the game. This is called before actors' PreInitializeComponents. */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** update remaining time */
	virtual void DefaultTimer();

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;
	
	UPROPERTY(BlueprintReadWrite, Category = GameTimeRules)
	int32 WarmupTime;

	/** match duration */
	UPROPERTY(BlueprintReadWrite, Category = GameTimeRules)
	int32 RoundTime;

	
};
