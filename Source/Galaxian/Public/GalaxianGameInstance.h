// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GalaxianGameInstance.generated.h"

class AGalaxianCharacter;

/**
 * 
 */
UCLASS()
class GALAXIAN_API UGalaxianGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	TArray<AActor*> IgnoredActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, int32> LeaderBoard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Win;

	int32 CountDuplicates;
};
