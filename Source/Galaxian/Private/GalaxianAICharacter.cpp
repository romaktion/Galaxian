// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianAICharacter.h"
#include "GalaxianBlueprintFunctionLibrary.h"
#include "GalaxianGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GalaxianDamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GalaxianHealthComponent.h"

AGalaxianAICharacter::AGalaxianAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, RaidAttackImpactEffect(nullptr)
, IsRaidAttack(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Diplomacy = 1;

	GetCapsuleComponent()->SetCollisionProfileName("AICharacter");
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 750;
}

void AGalaxianAICharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AGalaxianAICharacter::OnHit);
}

void AGalaxianAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->AddInputVector(GetActorForwardVector());
}

void AGalaxianAICharacter::StartRaidAttack()
{
	if (IsRaidAttack)
		return;

	IsRaidAttack= true;

	SpawnDefaultController();
	GetCapsuleComponent()->SetCollisionProfileName("Projectile");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	SetActorTickEnabled(true);
}

void AGalaxianAICharacter::OnKilled()
{
	Super::OnKilled();

	SetLifeSpan(1.f);
}

void AGalaxianAICharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		auto HealthComp = OtherActor->FindComponentByClass<UGalaxianHealthComponent>();

		UGameplayStatics::ApplyPointDamage(OtherActor, HealthComp ? HealthComp->GetMaxHealth() : MAX_FLT, FVector::ZeroVector, Hit, GetController(), this, UGalaxianDamageType::StaticClass());
	}

	if (IsRaidAttack)
	{
		UGameplayStatics::ApplyDamage(this, GetMaxHealth(), GetController(), this, UGalaxianDamageType::StaticClass());

		MulticastRaidEffect();
	}
}

void AGalaxianAICharacter::MulticastRaidEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaidAttackImpactEffect, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
}
