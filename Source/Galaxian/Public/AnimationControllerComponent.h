// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "COmponents/SkeletalMeshComponent.h"
#include "GalaxianCharacter.h"
#include "AnimationControllerComponent.generated.h"


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
	FORCEINLINE FVector GetVelocity() const
	{
		if (!GetOwner())
			return FVector::ZeroVector;

		if (!MovementComponent)
		{
			auto MoveComp = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
			if (MoveComp)
			{
				MovementComponent = MoveComp;
				return MovementComponent->Velocity;
			}
			else
			{
				return FVector::ZeroVector;
			}
		}
		else
		{
			return MovementComponent->Velocity;
		}
	}

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE AGalaxianCharacter* GetCharacter() const
	{
		return Cast<AGalaxianCharacter>(GetOwner());
	}

	UFUNCTION(BlueprintPure, Category = "Animation")
	FORCEINLINE USkeletalMeshComponent* GetMesh() const
	{
		auto Char = Cast<AGalaxianCharacter>(GetOwner());

		return Char ? Char->GetMesh() : nullptr;
	}

private:
	mutable UCharacterMovementComponent* MovementComponent;
};
