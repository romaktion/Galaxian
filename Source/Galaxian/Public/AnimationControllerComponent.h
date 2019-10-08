// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimationControllerComponent.generated.h"

class UCharacterMovementComponent;
class AGalaxianCharacter;
class USkeletalMeshComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GALAXIAN_API UAnimationControllerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE FVector GetVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE AGalaxianCharacter* GetCharacter() const;

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE USkeletalMeshComponent* GetMesh() const;

private:
	mutable UCharacterMovementComponent* MovementComponent;
};
