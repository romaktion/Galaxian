// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GalaxianEnemyController.generated.h"

class AGalaxianCharacter;

UCLASS(Abstract)
class GALAXIAN_API AGalaxianEnemyController : public ACharacter
{
	GENERATED_UCLASS_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	TArray<TSubclassOf<AGalaxianCharacter>> EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	int32 AmountEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	int32 AmountLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	float Interval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	float AttackInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	float MoveInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	float ForwardSpeedScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyController")
	int32 RaidAttackInterval;

private:
	UFUNCTION()
	void StartFireTimer();

	UFUNCTION()
	void MoveSideTimer();

	UFUNCTION()
	void RaidAttackTimer();

	int32 GetRandom();

	UFUNCTION()
	void OnDestroyed(AActor* Actor);

	FTimerHandle StartFireTimerHandle;
	FTimerHandle MoveSideTimerHandle;
	FTimerHandle RaidAttackTimerHandle;

	UPROPERTY()
	TArray<AGalaxianCharacter*> Enemies;

	bool MoveSide;
};
