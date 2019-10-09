// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianGameState.h"
#include "GalaxianGameMode.h"
#include "GalaxianHealthComponent.h"
#include "GalaxianPlayerState.h"

AGalaxianGameState::AGalaxianGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AGalaxianGameState::AddEnemy(AActor* InEnemy)
{
	Enemies.AddUnique(InEnemy);
	auto HealthComponent = InEnemy->FindComponentByClass<UGalaxianHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->OnKilled.AddUniqueDynamic(this, &AGalaxianGameState::OnEnemyKilledImpl);
	}
}

void AGalaxianGameState::RemoveEnemy(AActor* InEnemy)
{
	Enemies.Remove(InEnemy);
	if (Enemies.Num() <= 0)
	{
		auto World = GetWorld();
		if (World)
		{
			auto GM = Cast<AGalaxianGameMode>(World->GetAuthGameMode());
			if (GM)
				GM->PerformGameOver(1);
		}
	}
}

void AGalaxianGameState::OnEnemyKilledImpl(AActor* KilledActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	auto PS = InstigatedBy ? Cast<AGalaxianPlayerState>(InstigatedBy->PlayerState) : nullptr;
	if (PS)
		PS->AddEnemyCounter();
}
