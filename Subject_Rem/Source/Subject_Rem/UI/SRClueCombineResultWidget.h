// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRClueCombineResultWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;
class UTexture2D;
class UButton;

UCLASS()
class SUBJECT_REM_API USRClueCombineResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetClueMapImage(UTexture2D* Icon);
	void SetClueMapDes(FName Text);
	void SetClueMapName(FName Text);
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickedCloseButton();

	UPROPERTY(meta = (BindWidget))
	UImage* ClueMapImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapName;

	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
};
