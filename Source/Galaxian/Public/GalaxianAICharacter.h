// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxianCharacter.h"
#include "GalaxianAICharacter.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GALAXIAN_API AGalaxianAICharacter : public AGalaxianCharacter
{
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	GENERATED_UCLASS_BODY()
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartRaidAttack();

	virtual void OnKilled() override;

	UPROPERTY(EditAnywhere)
	UParticleSystem* RaidAttackImpactEffect;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRaidEffect();

	bool IsRaidAttack;

};
