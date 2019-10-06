// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianCharacter.h"
#include "GalaxianWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GalaxianPlayerState.h"
#include "GalaxianHealthComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGalaxianCharacter::AGalaxianCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, WeaponClass(nullptr)
, WeaponSocketName(TEXT("WeaponSocket"))
, Weapon(nullptr)
, Diplomacy(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	GetMesh()->KinematicBonesUpdateType = EKinematicBonesUpdateToPhysics::SkipAllBones;
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<UGalaxianHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AGalaxianCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto World = GetWorld();
	if (World)
	{
		if (Role == ROLE_Authority)
		{
			auto Weap = World->SpawnActorDeferred<AGalaxianWeapon>(WeaponClass, FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (Weap)
			{
				UGameplayStatics::FinishSpawningActor(Weap, FTransform::Identity);

				if (GetMesh())
					Weap->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);

				SetWeapon(Weap);
			}
		}
	}
}

// Called every frame
void AGalaxianCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGalaxianCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AGalaxianCharacter::Move);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGalaxianCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AGalaxianCharacter::StopFire);
}

AGalaxianWeapon* AGalaxianCharacter::GetWeapon() const
{
	return Weapon;
}

float AGalaxianCharacter::GetMaxHealth() const
{
	return HealthComponent ? HealthComponent->GetMaxHealth() : 0.f;
}

float AGalaxianCharacter::GetHealth() const
{
	return HealthComponent ? HealthComponent->GetHealth() : 0.f;
}

void AGalaxianCharacter::SetMaxHealth(const float& NewMaxHealth)
{
	if (HealthComponent)
		HealthComponent->SetMaxHealth(NewMaxHealth);
}

void AGalaxianCharacter::SetHealth(const float& NewHealth)
{
	if (HealthComponent)
		HealthComponent->SetHealth(NewHealth);
}

void AGalaxianCharacter::SetWeapon(AGalaxianWeapon* InWeapon)
{
	if (Role == ROLE_Authority)
		Weapon = InWeapon;
}

int32 AGalaxianCharacter::GetDiplomacy() const
{
	return Diplomacy;
}

void AGalaxianCharacter::SetDiplomacy(const int32& NewDiplomacy)
{
	if (Role == ROLE_Authority)
		Diplomacy = NewDiplomacy;
}

void AGalaxianCharacter::Move(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AGalaxianCharacter::StartFire()
{
	if (Weapon)
		Weapon->StartFire();
}

void AGalaxianCharacter::StopFire()
{
	if (Weapon)
		Weapon->StopFire();
}

void AGalaxianCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGalaxianCharacter, Weapon);
	DOREPLIFETIME(AGalaxianCharacter, Diplomacy);
}