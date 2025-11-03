// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class LEGACYFANTASY_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* HPText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LevelText;

	void SetHP(int NewHP);
	void SetLevel(int NewLevel);
};
