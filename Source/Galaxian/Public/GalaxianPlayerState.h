// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GalaxianPlayerState.generated.h"

class AGalaxianCharacter;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnenemyKilledSignature, AGalaxianPlayerState, OnEnemyKilled);

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
	
	virtual void CopyProperties(APlayerState* InPlayerState) override;

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	EPlayerStateEnum GetPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetPlayerState(EPlayerStateEnum NewPlayerState);

	UFUNCTION(NetMulticast, Reliable)
	void GoToMainMenu(bool Win);

	void AddEnemyCounter();

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	int32 GetEnemyCounter() const;

	virtual void OnRep_Score() override;

	UPROPERTY(Replicated)
	TSubclassOf<AGalaxianCharacter> PlayerPawnClass;

	UPROPERTY(BlueprintAssignable)
	FOnenemyKilledSignature OnEnemyKilled;

private:
	UPROPERTY(Replicated)
	EPlayerStateEnum PlayerState;
};
