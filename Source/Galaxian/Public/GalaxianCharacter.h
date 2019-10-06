// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GalaxianCharacter.generated.h"

class AGalaxianWeapon;
class UGalaxianHealthComponent;

UCLASS()
class GALAXIAN_API AGalaxianCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AGalaxianWeapon* GetWeapon() const;

	UFUNCTION(BlueprintPure, Category = "Character")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Character")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetMaxHealth(const float& NewMaxHealth);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetHealth(const float& NewHealth);

	void SetWeapon(AGalaxianWeapon* InWeapon);

	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetDiplomacy() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetDiplomacy(const int32& NewDiplomacy);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGalaxianHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGalaxianWeapon> WeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	FName WeaponSocketName;

private:
	void Move(float Value);

	void StartFire();

	void StopFire();

	UPROPERTY(Replicated)
	AGalaxianWeapon* Weapon;

	UPROPERTY(Replicated)
	int32 Diplomacy;
};
