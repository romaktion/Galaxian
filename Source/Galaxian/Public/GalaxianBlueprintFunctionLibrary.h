// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GalaxianBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EDiplomacyEnum : uint8
{
	DP_Friendly 	UMETA(DisplayName = "Friendly"),
	DP_Hostile 	UMETA(DisplayName = "Hostile")
};

/**
 * 
 */
UCLASS()
class GALAXIAN_API UGalaxianBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Configuration")
	static void SetStringToConfig(const FString& Section, const FString& ValueName, const FString& Value);

	UFUNCTION(BlueprintPure, Category = "Configuration")
	static FString GetStringFromConfig(const FString& Section, const FString& ValueName, bool& IfFind);

	UFUNCTION(BlueprintPure, Category = "Character")
	static EDiplomacyEnum GetDiplomacy(AActor* A, AActor* B);

	UFUNCTION(BlueprintPure, Category = "Character")
	static EDiplomacyEnum GetDiplomacyByValue(AActor* A, const int32& B);
};
