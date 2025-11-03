// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

void UCharacterHUD::SetHP(int NewHP)
{
	FString Str = FString::Printf(TEXT("HP: %d"), NewHP);
	HPText->SetText(FText::FromString(Str));
}

void UCharacterHUD::SetLevel(int NewLevel)
{
	FString Str = FString::Printf(TEXT("Level: %d"), NewLevel);
	LevelText->SetText(FText::FromString(Str));
}
