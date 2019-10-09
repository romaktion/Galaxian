// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GalaxianGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnGameOverSignature, AGalaxianGameMode, OnGameOver, bool, Win);

/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianGameMode : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual FString	InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	void PerformGameOver(int32 InWin);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Win;

	UPROPERTY(BlueprintAssignable)
	FOnGameOverSignature OnGameOver;

private:
	UFUNCTION()
	void OnKilled(AActor* PlayerCharacter);

	UFUNCTION()
	void GoToMainMenuTimer();

	FTimerHandle OnKilledTimerHandle;

	int32 CountPlayers;
};
