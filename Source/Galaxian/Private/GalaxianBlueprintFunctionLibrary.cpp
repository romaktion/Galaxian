// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianBlueprintFunctionLibrary.h"

UGalaxianBlueprintFunctionLibrary::UGalaxianBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGalaxianBlueprintFunctionLibrary::SetStringToConfig(const FString& Section, const FString& ValueName, const FString& Value)
{
	if (!GConfig)
		return;

	GConfig->SetString(*Section, *ValueName, *Value, GGameIni);
	GConfig ->Flush(false, GGameIni);
}

FString UGalaxianBlueprintFunctionLibrary::GetStringFromConfig(const FString& Section, const FString& ValueName, bool& IfFind)
{
	if (!GConfig)
		return FString("");

	FString NewValue = "";
	IfFind = GConfig->GetString(*Section, *ValueName, NewValue, GGameIni);
	return NewValue;
}
