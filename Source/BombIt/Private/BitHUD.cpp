// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitHUD.h"
#include "Engine/Canvas.h"


ABitHUD::ABitHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDCenterDotObj(TEXT("/Game/StarterContent/Textures/T_CenterDot_M.T_CenterDot_M"));
	CenterDotIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);

	CenterDotColor = { 255, 255, 255, 192 };

	CenterDotOn = true;
}

void ABitHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* MyPC = Cast<APlayerController>(GetOwningPlayerController());
	if (MyPC)
	{
		// enables input from player
		this->EnableInput(MyPC);
	}


}
void ABitHUD::DrawHUD()
{
	Super::DrawHUD();

	// crosshair
	if (CenterDotOn)
	{
		DrawCenterDot();
	}
}

void ABitHUD::DrawCenterDot()
{
	/** UI scaling factor for other resolutions than Full HD. */
	float ScaleUI = Canvas->ClipY / 1080.0f;

	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;

	float CenterDotScale = 0.07f;

	Canvas->SetDrawColor(CenterDotColor);

	Canvas->DrawIcon(CenterDotIcon,
		CenterX - CenterDotIcon.UL*CenterDotScale / 2.0f,
		CenterY - CenterDotIcon.VL*CenterDotScale / 2.0f, CenterDotScale);
}




