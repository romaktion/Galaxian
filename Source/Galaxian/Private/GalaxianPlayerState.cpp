// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPlayerState.h"
#include "GalaxianCharacter.h"
#include "UnrealNetwork.h"

AGalaxianPlayerState::AGalaxianPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, PlayerPawnClass(nullptr)
{
	bUseCustomPlayerNames = true;
}

void AGalaxianPlayerState::CopyProperties(APlayerState* PlayerState)
{
	auto SavedPlayerState = Cast<AGalaxianPlayerState>(PlayerState);
	ensureMsgf(SavedPlayerState != nullptr, TEXT("AGalaxianPlayerState::CopyProperties PlayerState cast failed"));
	
	SavedPlayerState->PlayerPawnClass = PlayerPawnClass;

	Super::CopyProperties(PlayerState);
}

void AGalaxianPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGalaxianPlayerState, PlayerPawnClass, COND_OwnerOnly);
}
