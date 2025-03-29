#include "MyBlueprintFunctionLibrary.h"

FString UMyBlueprintFunctionLibrary::GenerateAnswer()
{
	FString RandomNumber = "";
	TArray<int32> Numbers = { 1,2,3,4,5,6,7,8,9 };

	for (int32 i = Numbers.Num() - 1; i > 0; --i)
	{
		int32 Index = FMath::RandRange(0, i);
		Numbers.Swap(i, Index);
	}

	for (int32 i = 0; i < 3; ++i)
	{
		RandomNumber.Append(FString::FromInt(Numbers[i]));
	}

	return RandomNumber;
}

FString UMyBlueprintFunctionLibrary::JudgeGuess(const FString& Answer, const FString& Guess)
{
	int32 Strike = 0;
	int32 Ball = 0;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Guess);
	for (int32 i = 0; i < 3; ++i)
	{
		if (Answer[i] == Guess[i])
		{
			++Strike;
		}
		else if (Answer.Contains(FString::Chr(Guess[i])))
		{
			++Ball;
		}
	}
	if (Strike == 0 && Ball == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), Strike, Ball);
}
