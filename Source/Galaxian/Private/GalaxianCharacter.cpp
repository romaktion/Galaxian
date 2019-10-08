// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianCharacter.h"
#include "GalaxianWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GalaxianPlayerState.h"
#include "GalaxianHealthComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GalaxianPlayerController.h"
#include "GalaxianGameInstance.h"
#include "GalaxianBlueprintFunctionLibrary.h"

// Sets default values
AGalaxianCharacter::AGalaxianCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, WeaponClass(nullptr)
, WeaponSocketName(TEXT("WeaponSocket"))
, DestroyEffect(nullptr)
, Weapon(nullptr)
, Diplomacy(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	GetMesh()->KinematicBonesUpdateType = EKinematicBonesUpdateToPhysics::SkipAllBones;
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
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
				Weap->Diplomacy = GetDiplomacy();
				UGameplayStatics::FinishSpawningActor(Weap, FTransform::Identity);

				if (GetMesh())
					Weap->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);

				SetWeapon(Weap);
			}
		}
	}

	if (HealthComponent)
		HealthComponent->OnKilled.AddUniqueDynamic(this, &AGalaxianCharacter::OnKilled);

	GetMesh()->SetComponentTickEnabled(false);
	GetWorldTimerManager().SetTimer(MeshTickTimerHandler, this, &AGalaxianCharacter::MeshTickTimer, FMath::RandRange(0.f, 1.f), false, FMath::RandRange(0.f, 1.f));

	auto GI = Cast<UGalaxianGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->IgnoredActors.Add(this);
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

void AGalaxianCharacter::MeshTickTimer()
{
	GetMesh()->SetComponentTickEnabled(true);
}

void AGalaxianCharacter::MulticastDestroy_Implementation()
{
	if (Role == ROLE_Authority)
	{
		SetActorEnableCollision(false);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);
	}
	
	SetActorHiddenInGame(true);

	if (DestroyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
	}
}

void AGalaxianCharacter::OnKilled()
{
	auto PS = Cast<AGalaxianPlayerState>(GetPlayerState());

	if (PS)
	{
		PS->SetPlayerState(EPlayerStateEnum::PS_Killed);
	}

	auto PC = Cast<AGalaxianPlayerController>(GetController());
	if (PC)
		PC->OnCharacterKilled.Broadcast(this);

	MulticastDestroy();
	K2_OnKilled();
}

void AGalaxianCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGalaxianCharacter, Weapon);
	DOREPLIFETIME(AGalaxianCharacter, Diplomacy);
}