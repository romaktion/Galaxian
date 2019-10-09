// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianHealthComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UGalaxianHealthComponent::UGalaxianHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, MaxHealth(0.f)
, Health(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...

	SetIsReplicated(true);
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
	return MaxHealth;
}

float UGalaxianHealthComponent::GetHealth() const
{
	return Health;
}

bool UGalaxianHealthComponent::IsKilled() const
{
	return Health <= 0;
}

void UGalaxianHealthComponent::SetMaxHealth(const float& NewMaxHealth)
{
	if (GetOwner() && GetOwner()->Role == ROLE_Authority)
	{
		auto OldMaxHealth = MaxHealth;
		MaxHealth = NewMaxHealth;
		OnRep_MaxHealth(OldMaxHealth);
	}
}

void UGalaxianHealthComponent::SetHealth(const float& NewHealth)
{
	if (GetOwner() && GetOwner()->Role == ROLE_Authority)
	{
		auto OldHealth = Health;
		Health = NewHealth;
		OnRep_Health(OldHealth);
	}
}

void UGalaxianHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsKilled())
		return;

	SetHealth(FMath::Clamp(Health - Damage, 0.f, Health - Damage));
	if (IsKilled())
		OnKilled.Broadcast(DamagedActor, DamageType, InstigatedBy, DamageCauser);
}

void UGalaxianHealthComponent::OnRep_MaxHealth(float OldMaxHealth)
{
	OnChangeMaxHealth.Broadcast(MaxHealth - OldMaxHealth);
}

void UGalaxianHealthComponent::OnRep_Health(float OldHealth)
{
	OnChangeHealth.Broadcast(Health - OldHealth);
}

void UGalaxianHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGalaxianHealthComponent, MaxHealth);
	DOREPLIFETIME(UGalaxianHealthComponent, Health);
}