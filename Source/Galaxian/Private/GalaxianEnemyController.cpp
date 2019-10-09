// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianEnemyController.h"
#include "GalaxianCharacter.h"
#include "GalaxianWeapon.h"
#include "GalaxianAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GalaxianGameState.h"

// Sets default values
AGalaxianEnemyController::AGalaxianEnemyController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, AmountEnemies(0)
, AmountLines(0)
, Interval(0.f)
, AttackInterval(1.f)
, MoveInterval(5.f)
, ForwardSpeedScale(0.f)
, RaidAttackInterval(10)
, MoveSide(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 100.f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AGalaxianEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		auto World = GetWorld();
		if (World && AmountEnemies > 0 && EnemyTypes.Num() > 0)
		{
			bool Side = false;
			int32 Count = 0;
			int32 CountLines = 0;
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParameters.Owner = this;

			for (int i = 0; i < AmountLines; i++)
			{
				for (int32 j = 0; j < AmountEnemies; j++)
				{
					FTransform Transform;
					Transform.SetRotation(FQuat(GetActorRotation()));
					Transform.SetLocation((GetActorLocation() + GetActorRightVector() * Interval * Count * (Side ? 1 : -1)) + GetActorForwardVector() * Interval * 3 * CountLines);

					auto SpawnedEnemy = World->SpawnActor<AGalaxianCharacter>(EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)], Transform, ActorSpawnParameters);

					SpawnedEnemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

					SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &AGalaxianEnemyController::OnDestroyed);

					Enemies.Add(SpawnedEnemy);

					auto GS = Cast<AGalaxianGameState>(World->GetGameState());
					if (GS)
					{
						GS->AddEnemy(SpawnedEnemy);
					}

					Count++;
					Side = !Side;
				}

				Count = 0;
				CountLines++;
			}

			for (auto &E : Enemies)
			{
				E->GetCapsuleComponent()->MoveIgnoreActors.Append(Enemies);
			}
		}

		GetWorldTimerManager().SetTimer(StartFireTimerHandle, this, &AGalaxianEnemyController::StartFireTimer, AttackInterval, true);
		GetWorldTimerManager().SetTimer(MoveSideTimerHandle, this, &AGalaxianEnemyController::MoveSideTimer, MoveInterval / 2, false);
		GetWorldTimerManager().SetTimer(RaidAttackTimerHandle, this, &AGalaxianEnemyController::RaidAttackTimer, GetRandom(), false, GetRandom());

		SpawnDefaultController();
	}
}

// Called every frame
void AGalaxianEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->AddInputVector((GetActorRightVector() * (MoveSide ? 1 : -1)) + GetActorForwardVector() * ForwardSpeedScale);
}

void AGalaxianEnemyController::StartFireTimer()
{
	if (Enemies.Num() > 0)
	{
		auto Enemy = Enemies[FMath::RandRange(0, Enemies.Num() - 1)];

		if (Enemy)
		{
			Enemy->GetWeapon()->StartFire();
		}
	}
}

void AGalaxianEnemyController::MoveSideTimer()
{
	MoveSide = !MoveSide;
	GetWorldTimerManager().SetTimer(MoveSideTimerHandle, this, &AGalaxianEnemyController::MoveSideTimer, MoveInterval, false);
}

void AGalaxianEnemyController::RaidAttackTimer()
{
	if (Enemies.Num() > 0)
	{
		auto Player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
		if (Player)
		{
			auto MinDist = (Enemies[0]->GetActorLocation() - Player->GetActorLocation()).Size();
			int32 Index = 0;
			int32 Count = 0;

			for (auto &E : Enemies)
			{
				if (E && (E->GetActorLocation() - Player->GetActorLocation()).Size() < MinDist)
				{
					MinDist = (E->GetActorLocation() - Player->GetActorLocation()).Size();
					Index = Count;
				}

				Count++;
			}

			auto Enemy = Enemies[Index];

			if (Enemy)
			{
				auto EnemyChar = Cast<AGalaxianAICharacter>(Enemy);
				if (EnemyChar)
				{
					Enemies.Remove(EnemyChar);
					EnemyChar->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					EnemyChar->StartRaidAttack();
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(RaidAttackTimerHandle, this, &AGalaxianEnemyController::RaidAttackTimer, GetRandom(), false, GetRandom());
}

int32 AGalaxianEnemyController::GetRandom()
{
	return FMath::RandRange(RaidAttackInterval - RaidAttackInterval / 3, RaidAttackInterval - RaidAttackInterval + 3);
}

void AGalaxianEnemyController::OnDestroyed(AActor* Actor)
{
	auto Enemy = Cast<AGalaxianCharacter>(Actor);
	if (Enemy)
		Enemies.Remove(Enemy);

	auto World = GetWorld();
	if (World)
	{
		auto GS = Cast<AGalaxianGameState>(World->GetGameState());
		if (GS)
		{
			GS->RemoveEnemy(Actor);
		}
	}
}

