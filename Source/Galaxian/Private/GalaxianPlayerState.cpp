// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPlayerState.h"
#include "GalaxianCharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AGalaxianPlayerState::AGalaxianPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, PlayerPawnClass(nullptr)
, PlayerState(EPlayerStateEnum::PS_Alive)
{
	bUseCustomPlayerNames = true;
}

void AGalaxianPlayerState::CopyProperties(APlayerState* InPlayerState)
{
	auto SavedPlayerState = Cast<AGalaxianPlayerState>(InPlayerState);
	ensureMsgf(SavedPlayerState != nullptr, TEXT("AGalaxianPlayerState::CopyProperties PlayerState cast failed"));
	
	SavedPlayerState->PlayerPawnClass = PlayerPawnClass;

	Super::CopyProperties(InPlayerState);
}

EPlayerStateEnum AGalaxianPlayerState::GetPlayerState() const
{
	return PlayerState;
}

void AGalaxianPlayerState::SetPlayerState(EPlayerStateEnum NewPlayerState)
{
	if (Role == ROLE_Authority && PlayerState != NewPlayerState)
		PlayerState = NewPlayerState;
}

void AGalaxianPlayerState::GoToMainMenu_Implementation()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
}

void AGalaxianPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGalaxianPlayerState, PlayerState);

	DOREPLIFETIME_CONDITION(AGalaxianPlayerState, PlayerPawnClass, COND_OwnerOnly);
}
