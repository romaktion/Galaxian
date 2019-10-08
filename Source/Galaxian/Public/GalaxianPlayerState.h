// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GalaxianPlayerState.generated.h"

class AGalaxianCharacter;

UENUM(BlueprintType)
enum class EPlayerStateEnum : uint8
{
	PS_Alive 	UMETA(DisplayName = "Alive"),
	PS_Killed 	UMETA(DisplayName = "Killed")
};

/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(Replicated)
	TSubclassOf<AGalaxianCharacter> PlayerPawnClass;

	virtual void CopyProperties(APlayerState* InPlayerState) override;

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	EPlayerStateEnum GetPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetPlayerState(EPlayerStateEnum NewPlayerState);

	UFUNCTION(Client, Reliable)
	void GoToMainMenu();

private:
	UPROPERTY(Replicated)
	EPlayerStateEnum PlayerState;
};
