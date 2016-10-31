// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitPlayerController.h"


ABitPlayerController::ABitPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Bind Actions
	InputComponent->BindAction("FlushDebugLines", IE_Released, this, &ABitPlayerController::FlushDebugLines);

}

void ABitPlayerController::FlushDebugLines()
{
	FlushPersistentDebugLines(GetWorld());
}

