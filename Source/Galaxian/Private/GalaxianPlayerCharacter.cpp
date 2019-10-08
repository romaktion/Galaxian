// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPlayerCharacter.h"

AGalaxianPlayerCharacter::AGalaxianPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
}