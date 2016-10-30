// Fill out your copyright notice in the Description page of Project Settings.

#include "BombIt.h"
#include "BitBaseGameMode.h"
#include "BitGameState.h"
#include "BitPawn.h"

ABitBaseGameMode::ABitBaseGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/* Assign the class types used by this gamemode */

	/*// Player
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/LittleBird_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// HUD
	static ConstructorHelpers::FClassFinder<AHeliHud> HudBPClass(TEXT("/Game/Blueprints/HeliHud_BP"));
	if (HudBPClass.Class != NULL)
	{
		HUDClass = HudBPClass.Class;
	}

	PlayerControllerClass = AHeliPlayerController::StaticClass();
	PlayerStateClass = AHeliPlayerState::StaticClass();*/
	
	DefaultPawnClass = ABitPawn::StaticClass();
	GameStateClass = ABitGameState::StaticClass();
	WarmupTime = 0;
	RoundTime = 0;

}

void ABitBaseGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ABitBaseGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ABitBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	// TODO: game options
	RoundTime = UGameplayStatics::GetIntOption(Options, "RoundTime", 30);

	Super::InitGame(MapName, Options, ErrorMessage);

	bPauseable = true;
}

void ABitBaseGameMode::InitGameState()
{
	Super::InitGameState();

	// TODO: setting custom player configuration in the game state when hosting a match like custom server name, selected game mode, selected map, etc.
	// for displaying in scoreboard
	/*AHeliGameState* const MyGameState = Cast<AHeliGameState>(GameState);
	UHeliGameInstance* MyGameInstance = Cast<UHeliGameInstance>(GetGameInstance());
	if (MyGameState && MyGameInstance)
	{
		MyGameState->ServerName = MyGameInstance->CustomServerName;
		MyGameState->GameModeName = MyGameInstance->SelectedGameMode;
		MyGameState->MapName = MyGameInstance->SelectedMapName;
		MyGameState->MaxNumberOfPlayers = MyGameInstance->MaxNumberOfPlayers;
		MyGameState->MaxRoundTime = MyGameInstance->RoundTime;
		MyGameState->MaxWarmupTime = MyGameInstance->WarmupTime;

	}*/
}

void ABitBaseGameMode::DefaultTimer()
{
	// don't update timers for Play In Editor mode, it's not real match
	if (GetWorld()->IsPlayInEditor())
	{
		// start match if necessary.
		if (GetMatchState() == MatchState::WaitingToStart)
		{
			StartMatch();
		}
		return;
	}


	ABitGameState* const MyGameState = CastChecked<ABitGameState>(GameState);
	if (MyGameState && MyGameState->RemainingTime > 0)
	{
		MyGameState->RemainingTime--;

		if (MyGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				RestartGame();
			}
			else if (GetMatchState() == MatchState::InProgress)
			{
				EndMatch();
			}
			else if (GetMatchState() == MatchState::WaitingToStart)
			{
				StartMatch();
			}
		}
	}
}



