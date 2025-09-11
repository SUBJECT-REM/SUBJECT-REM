// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRCluemapCombinedDesWidget.generated.h"

/**
 * 
 */
class UImage;
class URichTextBlock;
class UButton;

UCLASS()
class SUBJECT_REM_API USRCluemapCombinedDesWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void SetLeftRightImage(TSoftObjectPtr<UTexture2D> Left, TSoftObjectPtr<UTexture2D> Right);
	void SetLeftRightItemName(FName Left, FName Right);
	void SetClueMapName(FName Name);
	void SetClueMapDes(FText Des);

	UFUNCTION()
	void OnCloseButtonClicked();
private:
	UPROPERTY(meta = (BindWidget))
	UImage* LeftImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RightImage;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* LeftImageItemName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* RightImageItemName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;
};
