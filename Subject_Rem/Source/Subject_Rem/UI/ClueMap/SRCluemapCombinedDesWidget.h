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

	void SetClueIcons(TArray<TSoftObjectPtr<UTexture2D>> Icons);
	void SetClueNamesText(const TArray<FText>& Text);
	void SetClueMapName(FText Name);
	void SetClueMapDes(FText Des);

	UFUNCTION()
	void OnCloseButtonClicked();
private:
	void SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D>& SoftTex);
	void SetRichText(URichTextBlock* Label, const FText& Text);
	void HideRichText(URichTextBlock* Label);
	void EnsureLayoutByCount(uint8 Count);
	// Switcher + 2개 레이아웃
	UPROPERTY(meta = (BindWidgetOptional)) class UWidgetSwitcher* LayoutSwitcher;
	UPROPERTY(meta = (BindWidgetOptional)) UImage* ClueLeftImage;   // 2개용 Left
	UPROPERTY(meta = (BindWidgetOptional)) UImage* ClueRightImage;  // 2개용 Right

	// 3개 레이아웃
	UPROPERTY(meta = (BindWidgetOptional)) UImage* ClueLeftImage_3;
	UPROPERTY(meta = (BindWidgetOptional)) UImage* ClueMidImage_3;
	UPROPERTY(meta = (BindWidgetOptional)) UImage* ClueRightImage_3;


	UPROPERTY(meta = (BindWidgetOptional)) URichTextBlock* LeftImageItemName;
	UPROPERTY(meta = (BindWidgetOptional)) URichTextBlock* RightImageItemName;

	UPROPERTY(meta = (BindWidgetOptional)) URichTextBlock* LeftImageItemName_3;
	UPROPERTY(meta = (BindWidgetOptional)) URichTextBlock* MidImageItemName_3;
	UPROPERTY(meta = (BindWidgetOptional)) URichTextBlock* RightImageItemName_3;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;
};
