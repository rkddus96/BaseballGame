#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballGameStateBase.h"
#include "BaseballGameController.generated.h"

class UGameWidget;

UCLASS()
class BASEBALLGAME_API ABaseballGameController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABaseballGameController();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	//State
	float TurnStartTime;
	float TurnDuration;
	FTimerHandle TurnUpdateHandle;


	bool bIsMyTurn;

	UPROPERTY(Replicated)
	int32 UserID;

	UPROPERTY(Replicated)
	bool bIsReady;

	UPROPERTY(Replicated)
	FString MyAnswer;

	//Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UGameWidget> GameWidgetClass;

	UPROPERTY()
	UGameWidget* GameWidgetInstance;

	FTimerHandle TurnTimerHandle;

	UFUNCTION(Server, Reliable)
	void Server_ToggleReady();

	UFUNCTION(Client, Reliable)
	void Client_StartTurn();

	UFUNCTION(Client, Reliable)
	void Client_WaitTurn();

	UFUNCTION(Client, Reliable)
	void Client_SetAnswer(const FString& Answer);

	UFUNCTION(Server, Reliable)
	void Server_SendGuess(const FString& Guess);

	UFUNCTION(Client, Reliable)
	void Client_ShowGuessResult(const FString& Guess, const FString& ResultText);

	UFUNCTION(Client, Reliable)
	void Client_ShowGameResult(EEndGameResult GameResult, const FString& GameAnswer);


	bool IsInputValid(const FString& InputText);
	void UpdateTurnUI();
	// --- UI Bindings ---
	UFUNCTION()
	void OnReadyButtonClicked();
	UFUNCTION()
	void OnGuessTextEnter(const FString& InputText);
};
