// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitGameState.h"


ABitGameState::ABitGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RemainingTime = 0;
	MaxRoundTime = 0;
}

