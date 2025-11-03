// Fill out your copyright notice in the Description page of Project Settings.


#include "LegacyFantasyGameInstance.h"

#include "Kismet/GameplayStatics.h"

int ULegacyFantasyGameInstance::GetCharacterHP()
{
	return CharacterHP;
}

void ULegacyFantasyGameInstance::SetCharacterHP(int NewHP)
{
	CharacterHP = NewHP;
}

void ULegacyFantasyGameInstance::ChangeLevel(int LevelIndex)
{
	if (LevelIndex <= 0) return;

	CurrentLevelIndex = LevelIndex;

	FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelNameString));
}

void ULegacyFantasyGameInstance::RestartGame()
{
	CharacterHP = 100;
	CurrentLevelIndex = 1;
	ChangeLevel(CurrentLevelIndex);
}
