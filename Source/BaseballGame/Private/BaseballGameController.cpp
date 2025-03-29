#include "BaseballGameController.h"
#include "GameWidget.h"
#include "BaseballGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyBlueprintFunctionLibrary.h"
#include "BaseballGameMode.h"
#include "Net/UnrealNetwork.h"

ABaseballGameController::ABaseballGameController() 
	: bIsMyTurn(false)
	, bIsReady(false)
	, MyAnswer(TEXT(""))
	, GameWidgetClass(nullptr)
	, GameWidgetInstance(nullptr)
	, UserID(-1)
	, TurnStartTime(0.0f)
	, TurnDuration(10.0f)
{
}

void ABaseballGameController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseballGameController, bIsReady);
	DOREPLIFETIME(ABaseballGameController, MyAnswer);
	DOREPLIFETIME(ABaseballGameController, UserID);
}

void ABaseballGameController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() && GameWidgetClass)
	{
		GameWidgetInstance = CreateWidget<UGameWidget>(this, GameWidgetClass);
		if (GameWidgetInstance)
		{
			GameWidgetInstance->AddToViewport();


			GameWidgetInstance->OnReadyClicked.BindUFunction(this, FName("OnReadyButtonClicked"));
			GameWidgetInstance->OnGuessEntered.BindUFunction(this, FName("OnGuessTextEnter"));

		}
	}
}


void ABaseballGameController::Server_ToggleReady_Implementation()
{
	ABaseballGameMode* GM = GetWorld()->GetAuthGameMode<ABaseballGameMode>();
	if (GM)
	{
		GM->PlayerReady(this); 
	}
}

void ABaseballGameController::Client_StartTurn_Implementation()
{
	bIsMyTurn = true;
	if (GameWidgetInstance)
	{
		GameWidgetInstance->EnableInput(true);
		GameWidgetInstance->SetStatusMessage(FString(TEXT("My Turn")));

		TurnStartTime = GetWorld()->GetTimeSeconds();
		TurnDuration = 10.0f;

		// 초기 UI 설정
		GameWidgetInstance->UpdateTurnTimer(TurnDuration, TurnDuration);
		GetWorldTimerManager().SetTimer(
			TurnUpdateHandle, this,
			&ABaseballGameController::UpdateTurnUI,
			0.1f, true
		);
	}
}

void ABaseballGameController::Client_WaitTurn_Implementation()
{
	bIsMyTurn = false;
	if (GameWidgetInstance)
	{
		GameWidgetInstance->EnableInput(false);
		GameWidgetInstance->SetStatusMessage(FString(TEXT("Opponent's Turn")));

		GetWorldTimerManager().ClearTimer(TurnUpdateHandle);

		if (GameWidgetInstance)
		{
			GameWidgetInstance->UpdateTurnTimer(0.f, 1.f); // 0초 남음으로 UI 리셋
		}
	}
}


void ABaseballGameController::Client_SetAnswer_Implementation(const FString& Answer)
{
	MyAnswer = Answer;
}

void ABaseballGameController::Server_SendGuess_Implementation(const FString& Guess)
{
	ABaseballGameMode* GM = GetWorld()->GetAuthGameMode<ABaseballGameMode>();
	if (GM)
	{
		GM->EvaluateGuess(this, Guess);
	}
}

void ABaseballGameController::Client_ShowGuessResult_Implementation(const FString& Guess, const FString& ResultText)
{
	if (GameWidgetInstance)
	{
		GameWidgetInstance->AddGuessText(Guess);
		GameWidgetInstance->AddResultText(ResultText);
	}
}




void ABaseballGameController::Client_ShowGameResult_Implementation(EEndGameResult GameResult,const FString& GameAnswer)
{
	if (GameWidgetInstance)
	{
		FString ResultText = TEXT("");

		if (GameResult == EEndGameResult::Draw)
		{
			ResultText = TEXT("DRAW");
		}
		else if ((GameResult == EEndGameResult::HostWin && UserID == 0) ||
				 (GameResult == EEndGameResult::GuestWin && UserID == 1))
		{
			ResultText = TEXT("WIN");
		}
		else
		{
			ResultText = TEXT("Lose");
		}
		
		GameWidgetInstance->ShowFinalResult(ResultText, GameAnswer);
		FTimerHandle ResetUITimer;
		GetWorld()->GetTimerManager().SetTimer(ResetUITimer, [this]()
			{
				if (GameWidgetInstance)
				{
					GameWidgetInstance->ResetUI();
				}
			}, 5.0f, false); // 3초 뒤 호출
	}
}

bool ABaseballGameController::IsInputValid(const FString& InputText)
{
	if (InputText.Len() != 3)
	{
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		TCHAR c = InputText[i];
		if (c < '1' || c > '9')
		{
			return false;
		}
	}

	return true;

}

void ABaseballGameController::UpdateTurnUI()
{
	float Elapsed = GetWorld()->GetTimeSeconds() - TurnStartTime;
	float Remaining = TurnDuration - Elapsed;

	if (Remaining < 0.0f)
	{
		Remaining = 0.0f;
		GetWorldTimerManager().ClearTimer(TurnUpdateHandle);
	}

	if (GameWidgetInstance)
	{
		GameWidgetInstance->UpdateTurnTimer(Remaining, TurnDuration);
	}
}

void ABaseballGameController::OnReadyButtonClicked()
{
	Server_ToggleReady();
}

void ABaseballGameController::OnGuessTextEnter(const FString& InputText)
{
	if (IsLocalController() && bIsMyTurn && IsInputValid(InputText))
	{
		Server_SendGuess(InputText);
		bIsMyTurn = false;
		GameWidgetInstance->EnableInput(false);  // UI 입력 막기
	}
}

