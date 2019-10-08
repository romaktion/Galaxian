// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GalaxianPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnCharacterKilledSignature, AGalaxianPlayerController, OnCharacterKilled, AActor*, PlayerCharacter);

/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "UserInterface")
	void PerformClientTravel(const FString& Path);

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FOnCharacterKilledSignature OnCharacterKilled;
};
