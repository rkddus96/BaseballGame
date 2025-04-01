#include "BaseballGameMode.h"
#include "BaseballGameStateBase.h"
#include "BaseballGameController.h"
#include "MyBlueprintFunctionLibrary.h"

ABaseballGameMode::ABaseballGameMode()
	: bHostReady(false)
	, bGuestReady(false)
	, HostAnswer(TEXT(""))
	, GuestAnswer(TEXT(""))
	, CurrentPlayerIndex(0)
{
	GameStateClass = ABaseballGameStateBase::StaticClass();
}

void ABaseballGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (ABaseballGameController* PC = Cast<ABaseballGameController>(NewPlayer))
	{
		PC->UserID = CurrentPlayerIndex++;
	}
	
}


void ABaseballGameMode::PlayerReady(APlayerController* Controller)
{
	ABaseballGameController* PC = Cast<ABaseballGameController>(Controller);
	if (PC)
	{
		PC->bIsReady = !PC->bIsReady;
	}

	bool bAllReady = true;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABaseballGameController* OtherPC = Cast<ABaseballGameController>(It->Get());
		if (!OtherPC || !OtherPC->bIsReady)
		{
			bAllReady = false;
			break;
		}
	}

	if (bAllReady)
	{
		StartGame();
	}

}

void ABaseballGameMode::StartGame()
{
	if (ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>())
	{
		GS->SetIsGameActive(true); // 시작 신호
	}

	TArray<ABaseballGameController*> PlayerList;

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (ABaseballGameController* PC = Cast<ABaseballGameController>(*it))
		{


			PlayerList.Add(PC);
		}

	}

	//답 생성
	for (int32 i = 0; i < PlayerList.Num(); ++i)
	{
		ABaseballGameController* PC = PlayerList[i];

		FString Answer = UMyBlueprintFunctionLibrary::GenerateAnswer();

		PC->Client_SetAnswer(Answer);

		if (PC->UserID == 0)
		{
			HostAnswer = Answer;
		}
		else
		{
			GuestAnswer = Answer;
		}
	}


	StartTurn();
}

void ABaseballGameMode::StartTurn()
{
	if (ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>())
	{
		bIsTurnEnded = false;

		const int32 CurrentTurn = GS->GetCurrentTurn();

		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		{
			if (ABaseballGameController* PC = Cast<ABaseballGameController>(it->Get()))
			{

				const bool bIsMyTurn = (CurrentTurn == 0 && PC->UserID == 0) || 
										(CurrentTurn == 1 && PC->UserID == 1);


				if (bIsMyTurn)
				{
					PC->bIsMyTurn = true;
					PC->Client_StartTurn();
				}
				else
				{
					PC->bIsMyTurn = false;
					PC->Client_WaitTurn();
				}
			}
		}

		GetWorldTimerManager().SetTimer(TurnTimer, this, &ABaseballGameMode::HandleTurnTimeout, 10.0f, false);
	}
}

void ABaseballGameMode::EndTurn()
{
	if (bIsTurnEnded)
		return;

	bIsTurnEnded = true;

	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS || !GS->GetIsGameActive())
		return;
	
	CheckGameResult();

	GS->SetCurrentTurn((GS->GetCurrentTurn() + 1) % 2);
	GS->SetTurnStatusText((GS->GetCurrentTurn() == 0) ? TEXT("Host's Turn") : TEXT("Guest's Turn"));

	StartTurn(); // 다음 턴 진입
}

void ABaseballGameMode::EvaluateGuess(APlayerController* Guesser, const FString& Guess)
{
	if (ABaseballGameController* PC = Cast<ABaseballGameController>(Guesser))
	{
		ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
		if (!GS || !GS->GetIsGameActive())
			return;

		FString OpponentAnswer;
			
		if (PC->UserID == 0)
		{
			OpponentAnswer = GuestAnswer;
			GS->SetHostTries(GS->GetHostTries() + 1);
		}
		else
		{
			OpponentAnswer = HostAnswer;
			GS->SetGuestTries(GS->GetGuestTries() + 1);
		}

		FString GuessResult = UMyBlueprintFunctionLibrary::JudgeGuess(OpponentAnswer, Guess);
		if (GuessResult == TEXT("3S0B"))
		{
			if (PC->UserID == 0)
			{
				GS->SetHostClearTurn(GS->GetHostTries());
			}
			else
			{
				GS->SetGuestClearTurn(GS->GetGuestTries());
			}
		}
		PC->Client_ShowGuessResult(Guess, GuessResult);

		GetWorldTimerManager().ClearTimer(TurnTimer);
		EndTurn(); 
	}
}

void ABaseballGameMode::CheckGameResult()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (GS->GetResult() != EEndGameResult::None)
		return;

	const int32 HostClearTurn = GS->GetHostClearTurn();
	const int32 GuestClearTurn = GS->GetGuestClearTurn();
	const int32 HostTries = GS->GetHostTries();
	const int32 GuestTries = GS->GetGuestTries();

	if (HostTries == GuestTries)
	{
		if (HostClearTurn == 0 && GuestClearTurn == 0 && HostTries >= 3 && GuestTries >= 3)
		{
			GS->SetResult(EEndGameResult::Draw);
			EndGame(nullptr);
			return;
		}
		
		if (HostClearTurn > 0 || GuestClearTurn > 0)
		{
			const int32 WinnerID = (HostClearTurn > 0) ? 0 : 1;
			GS->SetResult(WinnerID == 0 ? EEndGameResult::HostWin : EEndGameResult::GuestWin);

			for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
			{
				if (ABaseballGameController* PC = Cast<ABaseballGameController>(it->Get()))
				{
					if (PC->UserID == WinnerID)
					{
						EndGame(PC);
						return;
					}
				}
			}
		}
	}
}

void ABaseballGameMode::EndGame(APlayerController* Winner)
{
	if (ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>())
	{
		GS->SetIsGameActive(false);

		if (Winner == nullptr)
		{

		}

		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		{
			if (ABaseballGameController* PC = Cast<ABaseballGameController>(it->Get()))
			{
				if (PC->UserID == 0)
					PC->Client_ShowGameResult(GS->GetResult(), GuestAnswer);
				else
					PC->Client_ShowGameResult(GS->GetResult(), HostAnswer);
			}
		}

		FTimerHandle ResetGameDataTimer;
		GetWorldTimerManager().SetTimer(ResetGameDataTimer, [GS]()
			{
				GS->SetHostTries(0);
				GS->SetGuestTries(0);
				GS->SetCurrentTurn(0);
				GS->SetTurnStatusText(TEXT("Host's Turn"));
				GS->SetResult(EEndGameResult::None);
				GS->SetHostClearTurn(0);
				GS->SetGuestClearTurn(0);
			}, 4.5f, false); // 4.5초 후 초기화
	}
}

void ABaseballGameMode::ResetGame()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (GS)
	{
		GS->SetHostTries(0);
		GS->SetGuestTries(0);
		GS->SetIsGameActive(false);
		GS->SetCurrentTurn(0);
		GS->SetTurnStatusText(TEXT("Host's Turn"));
		GS->SetResult(EEndGameResult::None);
		GS->SetHostClearTurn(0);
		GS->SetGuestClearTurn(0);
	}
	
}

void ABaseballGameMode::HandleTurnTimeout()
{
	if (ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>())
	{
		const int32 CurrentTurn = GS->GetCurrentTurn();

		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		{
			if (ABaseballGameController* PC = Cast<ABaseballGameController>(it->Get()))
			{
				if (PC->UserID == CurrentTurn)
				{
					if (CurrentTurn == 0)
						GS->SetHostTries(GS->GetHostTries() + 1);
					else
						GS->SetGuestTries(GS->GetGuestTries() + 1);

					PC->Client_ShowGuessResult(TEXT("Time Out"), TEXT("OUT"));

					break;
				}
			}
		}

		EndTurn();
	}
}
