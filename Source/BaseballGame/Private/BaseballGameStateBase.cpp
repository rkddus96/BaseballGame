#include "BaseballGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "MyBlueprintFunctionLibrary.h"

ABaseballGameStateBase::ABaseballGameStateBase()
{
	HostTries = 0;
	GuestTries = 0;
	bIsGameActive = false;
	CurrentTurn = 0;
	TurnStatusText = TEXT("Waiting for players...");
}

void ABaseballGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseballGameStateBase, HostTries);
	DOREPLIFETIME(ABaseballGameStateBase, GuestTries);
	DOREPLIFETIME(ABaseballGameStateBase, bIsGameActive);
	DOREPLIFETIME(ABaseballGameStateBase, CurrentTurn);
	DOREPLIFETIME(ABaseballGameStateBase, TurnStatusText);
	DOREPLIFETIME(ABaseballGameStateBase, Result);
	DOREPLIFETIME(ABaseballGameStateBase, HostClearTurn);
	DOREPLIFETIME(ABaseballGameStateBase, GuestClearTurn);

}


void ABaseballGameStateBase::OnRep_TurnStatusText()
{
}

void ABaseballGameStateBase::OnRep_Winner()
{
}

