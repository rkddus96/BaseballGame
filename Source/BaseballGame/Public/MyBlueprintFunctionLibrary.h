#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"


UCLASS()
class BASEBALLGAME_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Baseball|Init")
	static FString GenerateAnswer();
	UFUNCTION(BlueprintCallable, Category = "Baseball|Rules")
	static FString JudgeGuess(const FString& Answer, const FString& Guess);
};
