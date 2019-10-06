// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GalaxianDamageType.h"

// Sets default values
AGalaxianProjectile::AGalaxianProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, Damage(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGalaxianProjectile::OnHit);	// set up a notification for when this component hits something blocking

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
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigatorController(), this, UGalaxianDamageType::StaticClass());
	
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		if (ProjectileMovement)
			ProjectileMovement->StopMovementImmediately();
	}

	OnHitDelegate.Broadcast(this);
}