// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPlayerState.h"
#include "GalaxianCharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GalaxianGameInstance.h"
#include "GalaxianPlayerController.h"

AGalaxianPlayerState::AGalaxianPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, PlayerPawnClass(nullptr)
, PlayerState(EPlayerStateEnum::PS_Alive)
{
	Score = 0.f;
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

void AGalaxianPlayerState::AddEnemyCounter()
{
	if (Role == ROLE_Authority)
	{
		Score += 1;
		OnRep_Score();
	}
 }

int32 AGalaxianPlayerState::GetEnemyCounter() const
{
	return Score;
}

void AGalaxianPlayerState::OnRep_Score()
{
	OnEnemyKilled.Broadcast();
	if (GetPawn())
	{
		auto PC = Cast<AGalaxianPlayerController>(GetPawn()->GetController());
		if (PC)
		{
			PC->OnEnemyKilled.Broadcast();
		}
	}
}

void AGalaxianPlayerState::GoToMainMenu_Implementation(bool Win)
{
	auto GI = Cast<UGalaxianGameInstance>(GetGameInstance());
	if (GI)
	{
		auto PN = GetPlayerName();
		auto Finded = GI->LeaderBoard.Find(GetPlayerName());
		if (Finded)
		{
			GI->CountDuplicates++;
			PN.AppendInt(GI->CountDuplicates);
		}

		GI->LeaderBoard.Add(PN, Score);
		GI->LeaderBoard.ValueSort([](int32 A, int32 B){ return A > B; });
		GI->Win = Win;
	}

	if (GetPawn() && GetPawn()->IsLocallyControlled())
		UGameplayStatics::OpenLevel(this, "MainMenu");
}

void AGalaxianPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGalaxianPlayerState, PlayerState);

	DOREPLIFETIME_CONDITION(AGalaxianPlayerState, PlayerPawnClass, COND_OwnerOnly);
}
