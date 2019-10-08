// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GalaxianCharacter.h"

// Sets default values for this component's properties
UAnimationControllerComponent::UAnimationControllerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UAnimationControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAnimationControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FORCEINLINE FVector UAnimationControllerComponent::GetVelocity() const
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

FORCEINLINE AGalaxianCharacter* UAnimationControllerComponent::GetCharacter() const
{
	return Cast<AGalaxianCharacter>(GetOwner());
}

FORCEINLINE USkeletalMeshComponent* UAnimationControllerComponent::GetMesh() const
{
	auto Char = Cast<AGalaxianCharacter>(GetOwner());

	return Char ? Char->GetMesh() : nullptr;
}
