// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GalaxianProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnHitSignature, AGalaxianProjectile, OnHitDelegate, AGalaxianProjectile*, Projectile);

UCLASS()
class GALAXIAN_API AGalaxianProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReInit(const FTransform NewTransform);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage;

	FOnHitSignature OnHitDelegate;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
