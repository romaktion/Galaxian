// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GalaxianGameState.generated.h"


/**
 * 
 */
UCLASS()
class GALAXIAN_API AGalaxianGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()
	
	void AddEnemy(AActor* InEnemy);

	void RemoveEnemy(AActor* InEnemy);

private:
	UFUNCTION()
	void OnEnemyKilledImpl(AActor* KilledActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TArray<AActor*> Enemies;
};
