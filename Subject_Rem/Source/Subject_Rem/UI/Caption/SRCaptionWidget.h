// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRCaptionWidget.generated.h"

/**
 * 
 */
class URichTextBlock;

UCLASS()
class SUBJECT_REM_API USRCaptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowCaptionRow(FName Row);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> CaptionDataTable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URichTextBlock> CatpionTextBlock;

private:
	void ClearTextBlock();

};
