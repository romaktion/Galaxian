// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianGameMode.h"
#include "GalaxianGameState.h"
#include "GalaxianPlayerState.h"
#include "GalaxianPlayerController.h"
#include "GalaxianCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObjectGlobals.h"

AGalaxianGameMode::AGalaxianGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = AGalaxianGameState::StaticClass();
	PlayerStateClass = AGalaxianPlayerState::StaticClass();
	PlayerControllerClass = AGalaxianPlayerController::StaticClass();
	DefaultPawnClass = AGalaxianCharacter::StaticClass();
}

UClass* AGalaxianGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AGalaxianPlayerState* PlayerState = nullptr;

	if (InController)
		PlayerState = Cast<AGalaxianPlayerState>(InController->PlayerState);

	if (PlayerState && PlayerState->PlayerPawnClass != nullptr)
		return PlayerState->PlayerPawnClass;

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

FString AGalaxianGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal /*= TEXT("")*/)
{
	FString ClassName = *UGameplayStatics::ParseOption(Options, TEXT("PlayerPawnClass"));

	UClass* PlayerPawnClass = nullptr;

	if (!ClassName.IsEmpty())
	{
		UObject* ClassPackage = ANY_PACKAGE;
		PlayerPawnClass = (UClass*)StaticLoadObject(UClass::StaticClass(), nullptr, *ClassName);
	}
	
	auto PlayerState = Cast<AGalaxianPlayerState>(NewPlayerController->PlayerState);

	if (PlayerState != nullptr)
	{
		PlayerState->PlayerPawnClass = PlayerPawnClass;
	}

	FString OptionsCopy = Options;

	FString PlayerName = *UGameplayStatics::ParseOption(Options, TEXT("PlayerName"));
	if (!PlayerName.IsEmpty())
	{
		auto grab = true;
		FString Res;
		FURL Url;

		while (grab)
		{
			grab = UGameplayStatics::GrabOption(OptionsCopy, Res);
			if (grab && !Res.Contains("Name="))
				Url.AddOption(*Res);
		}
		
		for (auto &O : Url.Op)
		{
			OptionsCopy.Append(FString::Printf(TEXT("?%s"), *O));
		}

		OptionsCopy.Append(FString::Printf(TEXT("?Name=%s"), *PlayerName));
	}

	return Super::InitNewPlayer(NewPlayerController, UniqueId, OptionsCopy, Portal);
}
