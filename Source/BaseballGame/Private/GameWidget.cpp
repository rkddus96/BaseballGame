#include "GameWidget.h"
#include "BaseballGameController.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
//Components
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ProgressBar.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();


	InputBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputBox")));
	StatusText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StatusText")));
	ReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("ReadyButton")));
	StatusText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StatusText")));
	GuessBoard = Cast<UVerticalBox>(GetWidgetFromName(TEXT("GuessBoard")));
	ResultBoard = Cast<UVerticalBox>(GetWidgetFromName(TEXT("ResultBoard")));
	TimeProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("TimeProgressBar")));
	TimeRemainingText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimeRemainingText")));

	InputBox->OnTextCommitted.AddDynamic(this, &UGameWidget::OnInputCommitted);
	ReadyButton->OnClicked.AddDynamic(this, &UGameWidget::OnReadyButtonClicked);

}

void UGameWidget::AddGuessText(const FString& GuessResult)
{
	if (GuessBoard)
	{
		UTextBlock* NewText = NewObject<UTextBlock>(this);
		NewText->SetText(FText::FromString(GuessResult));
		NewText->SetJustification(ETextJustify::Center);
		FSlateFontInfo FontInfo = NewText->GetFont();
		FontInfo.Size = 40;
		NewText->SetFont(FontInfo);
		UVerticalBoxSlot* MySlot = Cast<UVerticalBoxSlot>(GuessBoard->AddChildToVerticalBox(NewText));
		if (MySlot)
		{
			MySlot->SetPadding(FMargin(10.f));
		}
	}
}

void UGameWidget::AddResultText(const FString& ResultText)
{
	if (ResultBoard)
	{
		UTextBlock* NewText = NewObject<UTextBlock>(this);
		NewText->SetText(FText::FromString(ResultText));
		NewText->SetJustification(ETextJustify::Center);
		FSlateFontInfo FontInfo = NewText->GetFont();
		FontInfo.Size = 40;
		NewText->SetFont(FontInfo);
		UVerticalBoxSlot* MySlot = Cast<UVerticalBoxSlot>(ResultBoard->AddChildToVerticalBox(NewText));
		if (MySlot)
		{
			MySlot->SetPadding(FMargin(10.f));
		}
	}
}

void UGameWidget::ShowFinalResult(const FString& Text, const FString& GameAnswer)
{
	UTextBlock* NoticeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NoticeText")));
	if (NoticeText)
	{
		NoticeText->SetText(FText::FromString(Text));
		NoticeText->SetVisibility(ESlateVisibility::Visible);
	}

	UTextBlock* AnswerText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AnswerText")));
	if (AnswerText)
	{
		AnswerText->SetText(FText::FromString(GameAnswer));
		AnswerText->SetVisibility(ESlateVisibility::Visible);
	}

	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TempHandle,
		[this, NoticeText, AnswerText]() {
			NoticeText->SetVisibility(ESlateVisibility::Hidden);
			AnswerText->SetVisibility(ESlateVisibility::Hidden);
		},
		3.0f,
		false);
}

void UGameWidget::ShowStartNotice()
{

	UTextBlock* NoticeText =  Cast<UTextBlock>(GetWidgetFromName(TEXT("NoticeText")));
	NoticeText->SetText(FText::FromString(TEXT("게임 시작!")));
	NoticeText->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TempHandle,
		[this, NoticeText]() {
			NoticeText->SetVisibility(ESlateVisibility::Hidden);
		},
		3.0f,
		false);
}

void UGameWidget::ResetUI()
{
	// 입력창 초기화
	if (InputBox)
	{
		InputBox->SetText(FText::GetEmpty());
	}

	// 상태 텍스트 초기화
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("")));
	}

	// 보드 초기화
	if (GuessBoard)
	{
		GuessBoard->ClearChildren();
	}

	if (ResultBoard)
	{
		ResultBoard->ClearChildren();
	}


	// 준비 버튼 다시 보이기 (옵션)
	if (ReadyButton)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		ReadyButton->SetIsEnabled(true);
	}

	// 입력창 비활성화
	EnableInput(false);
}

void UGameWidget::EnableInput(bool bEnabled)
{
	if (InputBox)
	{
		InputBox->SetIsEnabled(bEnabled);

		if (bEnabled)
		{
			InputBox->SetUserFocus(GetOwningPlayer());
		}
	}


}

void UGameWidget::SetStatusMessage(const FString& NewMessage)
{
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(NewMessage));
	}
}

void UGameWidget::UpdateTurnTimer(float RemainingTime, float TotalTime)
{
	if (TimeProgressBar)
	{
		TimeProgressBar->SetPercent(RemainingTime / TotalTime);
	}

	if (TimeRemainingText)
	{
		int32 SecondsLeft = FMath::CeilToInt(RemainingTime);
		TimeRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%ds"), SecondsLeft)));
	}
}

void UGameWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString InputString = Text.ToString();

		if (OnGuessEntered.IsBound())
		{
			OnGuessEntered.Execute(InputString);
		}

		InputBox->SetText(FText::GetEmpty());
	}
}

void UGameWidget::OnReadyButtonClicked()
{
	if (OnReadyClicked.IsBound())
	{
		ABaseballGameController* PC = Cast<ABaseballGameController>(GetOwningPlayer());
		if (PC)
		{
			bool IsReady = PC->bIsReady;
			UTextBlock* ReadyButtonText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ReadyButtonText")));
			if (IsReady)
			{
				ReadyButtonText->SetText(FText::FromString(TEXT("Cancel")));

			}
			else
			{
				ReadyButtonText->SetText(FText::FromString(TEXT("ready")));
			}

			OnReadyClicked.Execute();
		}

	}
}

