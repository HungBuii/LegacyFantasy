// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LegacyFantasyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEGACYFANTASY_API ULegacyFantasyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int CharacterHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	int CurrentLevelIndex = 1;

public:
	int GetCharacterHP();
	
	void SetCharacterHP(int NewHP);

	void ChangeLevel(int LevelIndex);

	UFUNCTION(BlueprintCallable)
	void RestartGame();
};
