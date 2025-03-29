#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseballGameMode.generated.h"


class ABaseballGameController;


UCLASS()
class BASEBALLGAME_API ABaseballGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABaseballGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void PlayerReady(APlayerController* Controller);
	void StartGame();
	void StartTurn();
	void EndTurn();
	void EvaluateGuess(APlayerController* Guesser, const FString& Guess);
	void CheckGameResult();
	void EndGame(APlayerController* Winner);
	void ResetGame();
	void HandleTurnTimeout();
protected:

	bool bHostReady;
	bool bGuestReady;
	FString HostAnswer;
	FString GuestAnswer;

	FTimerHandle TurnTimer;
	bool bIsTurnEnded = false;

	UPROPERTY()
	int32 CurrentPlayerIndex;
};

