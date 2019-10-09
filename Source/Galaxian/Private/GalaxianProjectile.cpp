// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GalaxianDamageType.h"
#include "GalaxianBlueprintFunctionLibrary.h"
#include "GalaxianGameInstance.h"

// Sets default values
AGalaxianProjectile::AGalaxianProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, Damage(0.f)
, DamageRadius(0.f)
, ImpactEffect(nullptr)
, Diplomacy(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGalaxianProjectile::OnHit);	// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGalaxianProjectile::OnOverlap);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetGenerateOverlapEvents(false);
	CollisionComp->SetCanEverAffectNavigation(false);

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AGalaxianProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->MoveIgnoreActors.Add(GetInstigator());
	CollisionComp->MoveIgnoreActors.Add(GetOwner());

	auto GI = Cast<UGalaxianGameInstance>(GetGameInstance());
	if (GI)
	{
		for (auto &IA : GI->IgnoredActors)
		{
			if (UGalaxianBlueprintFunctionLibrary::GetDiplomacyByValue(IA, Diplomacy) == EDiplomacyEnum::DP_Friendly)
			{
				CollisionComp->MoveIgnoreActors.Add(IA);
			}
		}
	}
}

// Called every frame
void AGalaxianProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGalaxianProjectile::ReInit(const FTransform NewTransform)
{
	SetActorTransform(NewTransform);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
		ProjectileMovement->UpdateComponentVelocity();
		ProjectileMovement->SetUpdatedComponent(CollisionComp);
	}
}

void AGalaxianProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitImpl(OtherActor, Hit, true);
}

void AGalaxianProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	HitImpl(OtherActor, SweepResult, false);
}

void AGalaxianProjectile::HitImpl(AActor* OtherActor, const FHitResult& Hit, bool IsBlock)
{
	if (!IsBlock && UGalaxianBlueprintFunctionLibrary::GetDiplomacyByValue(OtherActor, Diplomacy) == EDiplomacyEnum::DP_Friendly)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);

	if (ImpactSound.Num() > 0)
	{
		auto S = ImpactSound[FMath::RandRange(0, ImpactSound.Num() - 1)];
		if (S)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), S, GetActorLocation());
		}
	}

	if (OtherActor)
	{
		if (DamageRadius <= 0)
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, FVector::ZeroVector, Hit, GetInstigatorController(), this, UGalaxianDamageType::StaticClass());
		else
			UGameplayStatics::ApplyRadialDamage(this, Damage * 2.5f, GetActorLocation(), DamageRadius, UGalaxianDamageType::StaticClass(), TArray<AActor*>(), this, GetInstigatorController(), false, ECC_WorldStatic);

		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		if (ProjectileMovement)
			ProjectileMovement->StopMovementImmediately();
	}

	OnHitDelegate.Broadcast(this);
}
