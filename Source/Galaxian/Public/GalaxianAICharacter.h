// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxianCharacter.h"
#include "GalaxianAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnOnStartRaidAttackSignature, AGalaxianAICharacter, OnStartRaidAttack);

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

	virtual void OnKilled(AActor* KilledActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	UParticleSystem* RaidAttackImpactEffect;

	UPROPERTY(BlueprintAssignable)
	FOnOnStartRaidAttackSignature OnStartRaidAttack;

	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> OnStartRaidAttackSound;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRaidEffect();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnStartRaidAttack();

	bool IsRaidAttack;

};
