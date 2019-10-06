// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianWeapon.h"
#include "TimerManager.h"
#include "GalaxianProjectile.h"

// Sets default values
AGalaxianWeapon::AGalaxianWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, FireRate(0.f)
, ProjectileClass(nullptr)
, LastTimeFire(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGalaxianWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGalaxianWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGalaxianWeapon::StartFire()
{
	if (Role < ROLE_Authority)
	{
		StartFireTimer();
		ServerStartFire();
	}
	else
	{
		MulticastStartFire();
	}
}

void AGalaxianWeapon::StopFire()
{
	StopFireTimer();

	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}
	else
	{
		MulticastStopFire();
	}
}

void AGalaxianWeapon::Fire()
{
	auto World = GetWorld();
	if (!World)
		return;

	SpawnProjectile();

	LastTimeFire = World->GetTimeSeconds();
}

void AGalaxianWeapon::ServerStartFire_Implementation()
{
	StartFireTimer();
}

bool AGalaxianWeapon::ServerStartFire_Validate()
{
	return true;
}

void AGalaxianWeapon::ServerStopFire_Implementation()
{
	StopFireTimer();
}

bool AGalaxianWeapon::ServerStopFire_Validate()
{
	return true;
}

void AGalaxianWeapon::MulticastStartFire_Implementation()
{
	StartFireTimer();
}

bool AGalaxianWeapon::MulticastStartFire_Validate()
{
	return true;
}

void AGalaxianWeapon::MulticastStopFire_Implementation()
{
	StopFireTimer();
}

bool AGalaxianWeapon::MulticastStopFire_Validate()
{
	return true;
}

void AGalaxianWeapon::SpawnProjectile()
{
	auto World = GetWorld();

	if (!World)
		return;

	FTransform Transform;
	Transform.SetRotation(FQuat(GetActorRotation()));
	Transform.SetLocation(GetActorLocation());

	if (ProjectilePool.Num() > 0)
	{
		auto Projectile = ProjectilePool.Pop();
		if (Projectile)
		{
			Projectile->ReInit(Transform);
			return;
		}
	}

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameters.Owner = this;
	ActorSpawnParameters.Instigator = Cast<APawn>(GetOwner());

	auto Projectile = World->SpawnActor<AGalaxianProjectile>(ProjectileClass, Transform, ActorSpawnParameters);
	if (Projectile)
	{
		Projectile->OnHitDelegate.AddUniqueDynamic(this, &AGalaxianWeapon::OnProjectileHit);
	}
}

void AGalaxianWeapon::StartFireTimer()
{
	auto World = GetWorld();
	if (!World)
		return;

	float DelayBetweenShots = 60 / FireRate;

	float FirstDelay = LastTimeFire > 0 ? FMath::Max(DelayBetweenShots - (World->GetTimeSeconds() - LastTimeFire), 0.0f) : LastTimeFire;
	UE_LOG(LogTemp, Log, TEXT("FirstDelay: %i"), (int)FirstDelay);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AGalaxianWeapon::Fire, DelayBetweenShots, true, FirstDelay);
}

void AGalaxianWeapon::StopFireTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AGalaxianWeapon::OnProjectileHit(AGalaxianProjectile* Projectile)
{
	if (!Projectile)
		return;

	Projectile->SetActorEnableCollision(false);
	Projectile->SetActorHiddenInGame(true);
	ProjectilePool.Push(Projectile);
}
