#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballGameStateBase.h"
#include "Delegates/Delegate.h"
#include "UObject/ScriptDelegates.h"
#include "GameWidget.generated.h"


class UEditableTextBox;
class UButton;
class UTextBlock;
class UVerticalBox;
class UProgressBar;


//define Delegate
DECLARE_DYNAMIC_DELEGATE(FOnReadyClicked);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGuessEntered, const FString&, GuessText);

UCLASS()
class BASEBALLGAME_API UGameWidget : public UUserWidget
{
    GENERATED_BODY()
     
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* InputBox;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* StatusText;
    UPROPERTY(meta = (BindWidget))
    UButton* ReadyButton;
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* GuessBoard;
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ResultBoard;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* TimeProgressBar;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimeRemainingText;
 

    FOnReadyClicked OnReadyClicked;
    FOnGuessEntered OnGuessEntered;

    UFUNCTION(BlueprintCallable)
    void AddGuessText(const FString& GuessResult);

    UFUNCTION(BlueprintCallable)
    void AddResultText(const FString& ResultText);

    UFUNCTION(BlueprintCallable)
    void ShowFinalResult(const FString& ResultText, const FString& GameAnswer);

    UFUNCTION(BlueprintCallable)
    void ShowStartNotice();

    UFUNCTION(BlueprintCallable)
    void ResetUI();

    UFUNCTION(BlueprintCallable)
    void EnableInput(bool bEnabled);

    UFUNCTION(BlueprintCallable)
    void SetStatusMessage(const FString& NewMessage);

    void UpdateTurnTimer(float RemainingTime, float TotalTime);

    // Event
    UFUNCTION()
    void OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
    UFUNCTION()
    void OnReadyButtonClicked();

};