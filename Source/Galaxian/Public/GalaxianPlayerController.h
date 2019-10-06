// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GalaxianPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "UserInterface")
	void PerformClientTravel(const FString& Path);
};
