// Fill out your copyright notice in the Description page of Project Settings.


//#include "LobbyGameMode.h"
//#include "GameFramework/GameStateBase.h"
//
//void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
//	if (NumberOfPlayers >= 2)
//	{
//		if (UWorld* World = GetWorld())
//		{
//			bUseSeamlessTravel = true;
//			World->ServerTravel(FString("/Game/Maps/RaceMap?listen"));
//		}
//	}
//}

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

int32 ALobbyGameMode::GetExpectedPlayerCount() const
{
	// Fallback if session settings cannot be found
	int32 ExpectedPlayers = 2;

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return ExpectedPlayers;

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return ExpectedPlayers;

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!ExistingSession) return ExpectedPlayers;

	ExpectedPlayers = ExistingSession->SessionSettings.NumPublicConnections;

	return ExpectedPlayers;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!GameState || bStartedTravel) return;

	const int32 CurrentPlayers = GameState->PlayerArray.Num();
	const int32 ExpectedPlayers = GetExpectedPlayerCount();

	UE_LOG(LogTemp, Warning, TEXT("Lobby Players: %d / %d"), CurrentPlayers, ExpectedPlayers);

	if (CurrentPlayers >= ExpectedPlayers)
	{
		if (UWorld* World = GetWorld())
		{
			bStartedTravel = true;
			bUseSeamlessTravel = true;

			World->ServerTravel(FString("/Game/Maps/RaceMap?listen"));
		}
	}
}