// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GalaxianBarrier.generated.h"

class UGalaxianHealthComponent;

UCLASS(Abstract)
class GALAXIAN_API AGalaxianBarrier : public AActor
{
	GENERATED_UCLASS_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGalaxianHealthComponent* HealthComponent;

	UFUNCTION(BlueprintPure, Category = "Character")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Character")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetMaxHealth(const float& NewMaxHealth);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetHealth(const float& NewHealth);

};
