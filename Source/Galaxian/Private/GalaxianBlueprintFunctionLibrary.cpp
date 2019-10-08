// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianBlueprintFunctionLibrary.h"
#include "GalaxianCharacter.h"


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

EDiplomacyEnum UGalaxianBlueprintFunctionLibrary::GetDiplomacy(AActor* A, AActor* B)
{
	auto AChar = Cast<AGalaxianCharacter>(A);
	auto BChar = Cast<AGalaxianCharacter>(B);
	if (AChar && BChar)
	{
		return AChar->GetDiplomacy() == BChar->GetDiplomacy() ? EDiplomacyEnum::DP_Friendly : EDiplomacyEnum::DP_Hostile;
	}
	else
	{
		return EDiplomacyEnum::DP_Hostile;
	}
}

EDiplomacyEnum UGalaxianBlueprintFunctionLibrary::GetDiplomacyByValue(AActor* A, const int32& B)
{
	auto AChar = Cast<AGalaxianCharacter>(A);
	if (AChar)
	{
		return AChar->GetDiplomacy() == B ? EDiplomacyEnum::DP_Friendly : EDiplomacyEnum::DP_Hostile;
	}
	else
	{
		return EDiplomacyEnum::DP_Hostile;
	}
}
