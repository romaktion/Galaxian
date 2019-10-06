// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPlayerController.h"
#include "Engine/World.h"

AGalaxianPlayerController::AGalaxianPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AGalaxianPlayerController::PerformClientTravel(const FString& Path)
{
	ClientTravel(Path, ETravelType::TRAVEL_Absolute);
}
