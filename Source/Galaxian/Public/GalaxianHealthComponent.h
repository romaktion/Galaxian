// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GalaxianHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnKilledSignature, UGalaxianHealthComponent, OnKilled);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GALAXIAN_API UGalaxianHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	// Sets default values for this component's properties
	UGalaxianHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	bool IsKilled() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetMaxHealth(const float& NewMaxHealth);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetHealth(const float& NewHealth);

	UPROPERTY(BlueprintAssignable, Category="Character")
	FOnKilledSignature OnKilled;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly)
	float Health;

private:

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );

	UPROPERTY(Replicated)
	float MaxHealthPrivate;

	UPROPERTY(Replicated)
	float HealthPrivate;
};