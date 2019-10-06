// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GalaxianWeapon.generated.h"

class AGalaxianProjectile;

UCLASS()
class GALAXIAN_API AGalaxianWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartFire();

	void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGalaxianProjectile> ProjectileClass;

private:
	UFUNCTION()
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastStartFire();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastStopFire();

	void SpawnProjectile();

	void StartFireTimer();

	void StopFireTimer();

	UFUNCTION()
	void OnProjectileHit(AGalaxianProjectile* Projectile);

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastTimeFire;

	TArray<AGalaxianProjectile*> ProjectilePool;
};
