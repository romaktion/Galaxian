// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianWeapon.h"
#include "TimerManager.h"
#include "GalaxianProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "UnrealNetwork.h"

// Sets default values
AGalaxianWeapon::AGalaxianWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, FireRate(0.f)
, ProjectileClass(nullptr)
, AutomaticWeapon(false)
, MuzzleEffect(nullptr)
, Diplomacy(0)
, ManualShot(false)
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
	if (!this || !GetWorld())
		return;

	if (GetWorldTimerManager().IsTimerActive(TimerHandle_TimeBetweenShots))
		return;

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

	OnFire.Broadcast();

	if (!ManualShot)
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

void AGalaxianWeapon::MulticastStopFire_Implementation()
{
	StopFireTimer();
}

void AGalaxianWeapon::SpawnProjectile()
{
	auto World = GetWorld();

	if (!World)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleEffect, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);

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

	auto Projectile = World->SpawnActorDeferred<AGalaxianProjectile>(ProjectileClass, Transform, this, GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		Projectile->Diplomacy = Diplomacy;
		UGameplayStatics::FinishSpawningActor(Projectile, Transform);
		Projectile->OnHitDelegate.AddUniqueDynamic(this, &AGalaxianWeapon::OnProjectileHit);
	}
}

void AGalaxianWeapon::StartFireTimer()
{
	if (!this || !GetWorld())
		return;

	auto World = GetWorld();
	if (!World)
		return;

	float DelayBetweenShots = 60 / FireRate;

	if (AutomaticWeapon)
	{
		float FirstDelay = LastTimeFire > 0 ? FMath::Max(DelayBetweenShots - (World->GetTimeSeconds() - LastTimeFire), 0.0f) : LastTimeFire;
		GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AGalaxianWeapon::Fire, DelayBetweenShots, true, FirstDelay);
	}
	else
	{
		if (World->GetTimeSeconds() - LastTimeFire > DelayBetweenShots || LastTimeFire <= 0)
		{
			Fire();
		}
	}
}

void AGalaxianWeapon::StopFireTimer()
{
	if (!this || !GetWorld())
		return;

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

void AGalaxianWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGalaxianWeapon, Diplomacy);
}