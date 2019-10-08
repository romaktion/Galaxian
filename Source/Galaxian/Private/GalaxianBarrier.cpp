// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianBarrier.h"
#include "GalaxianHealthComponent.h"

// Sets default values
AGalaxianBarrier::AGalaxianBarrier(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UGalaxianHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AGalaxianBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGalaxianBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AGalaxianBarrier::GetMaxHealth() const
{
	return HealthComponent ? HealthComponent->GetMaxHealth() : 0.f;
}

float AGalaxianBarrier::GetHealth() const
{
	return HealthComponent ? HealthComponent->GetHealth() : 0.f;
}

void AGalaxianBarrier::SetMaxHealth(const float& NewMaxHealth)
{
	if (HealthComponent)
		HealthComponent->SetMaxHealth(NewMaxHealth);
}

void AGalaxianBarrier::SetHealth(const float& NewHealth)
{
	if (HealthComponent)
		HealthComponent->SetHealth(NewHealth);
}