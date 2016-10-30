// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "BitHUD.generated.h"

/**
 * 
 */
UCLASS()
class BOMBIT_API ABitHUD : public AHUD
{
	GENERATED_BODY()

	FCanvasIcon CenterDotIcon;

	void DrawCenterDot();

	// center dot color
	UPROPERTY(EditDefaultsOnly, Category = MouseCursorSettings, meta = (AllowPrivateAccess = "true"))
	FColor CenterDotColor;

public:
	ABitHUD();

	bool CenterDotOn;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	
	
};
