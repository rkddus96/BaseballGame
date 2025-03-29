#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseballGameStateBase.generated.h"

UENUM()
enum class EEndGameResult : uint8
{
	None     UMETA(DisplayName = "None"),
	HostWin  UMETA(DisplayName = "Host Wins!"),
	GuestWin UMETA(DisplayName = "Guest Wins!"),
	Draw     UMETA(DisplayName = "Draw Game")
};

UCLASS()
class BASEBALLGAME_API ABaseballGameStateBase : public AGameState
{
	GENERATED_BODY()
public:
	ABaseballGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_TurnStatusText();
	UFUNCTION()
	void OnRep_Winner();

	//getter, setter
	int32 GetHostTries() const { return HostTries; }
	int32 GetGuestTries() const { return GuestTries; }
	bool GetIsGameActive() const { return bIsGameActive; }
	int32 GetCurrentTurn() const { return CurrentTurn; }
	FString GetTurnStatusText() const { return TurnStatusText; }
	EEndGameResult GetResult() const { return Result; }
	int32 GetHostClearTurn() const { return HostClearTurn; }
	int32 GetGuestClearTurn() const { return GuestClearTurn; }

	void SetHostTries(int32 NewTries) { HostTries = NewTries; }
	void SetGuestTries(int32 NewTries) { GuestTries = NewTries; }
	void SetIsGameActive(bool NewActive) { bIsGameActive = NewActive; }
	void SetCurrentTurn(int32 NewTurn) { CurrentTurn = NewTurn; }
	void SetTurnStatusText(const FString& NewText) { TurnStatusText = NewText; }
	void SetResult(EEndGameResult NewResult) { Result = NewResult; }
	void SetHostClearTurn(int32 NewTurn) { HostClearTurn = NewTurn; }
	void SetGuestClearTurn(int32 NewTurn) { GuestClearTurn = NewTurn; }

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 HostTries;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 GuestTries;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	bool bIsGameActive;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentTurn; // 0: Host, 1: Guest

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 HostClearTurn = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 GuestClearTurn = 0;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_TurnStatusText, BlueprintReadOnly, Category = "Baseball")
	FString TurnStatusText;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Winner, BlueprintReadOnly, Category = "Baseball")
	EEndGameResult 	Result;
	
};
