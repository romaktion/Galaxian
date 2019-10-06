// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianHealthComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UGalaxianHealthComponent::UGalaxianHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, MaxHealth(0.f)
, Health(0.f)
, MaxHealthPrivate(MaxHealth)
, HealthPrivate(Health)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGalaxianHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner() && GetOwner()->Role == ROLE_Authority)
	{
		GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UGalaxianHealthComponent::OnTakeAnyDamage);
	}
}


// Called every frame
void UGalaxianHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UGalaxianHealthComponent::GetMaxHealth() const
{
	return MaxHealthPrivate;
}

float UGalaxianHealthComponent::GetHealth() const
{
	return HealthPrivate;
}

bool UGalaxianHealthComponent::IsKilled() const
{
	return HealthPrivate <= 0;
}

void UGalaxianHealthComponent::SetMaxHealth(const float& NewMaxHealth)
{
	if (GetOwner() && GetOwner()->Role == ROLE_Authority)
		MaxHealthPrivate = NewMaxHealth;
}

void UGalaxianHealthComponent::SetHealth(const float& NewHealth)
{
	if (GetOwner() && GetOwner()->Role == ROLE_Authority)
		HealthPrivate = NewHealth;
}

void UGalaxianHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsKilled())
		return;

	SetHealth(FMath::Clamp(HealthPrivate - Damage, 0.f, HealthPrivate - Damage));
	if (IsKilled())
		OnKilled.Broadcast();
}

void UGalaxianHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGalaxianHealthComponent, MaxHealthPrivate);
	DOREPLIFETIME(UGalaxianHealthComponent, HealthPrivate);
}