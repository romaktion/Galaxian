// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GalaxianPlayerState.generated.h"

class AGalaxianCharacter;

/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(Replicated)
	TSubclassOf<AGalaxianCharacter> PlayerPawnClass;

	virtual void CopyProperties(APlayerState* PlayerState) override;
};
