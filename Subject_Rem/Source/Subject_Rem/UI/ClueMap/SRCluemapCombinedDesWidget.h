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
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* IconLayoutSwitcher;

	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* ItemNameLayoutSwitcher;
	UPROPERTY(meta = (BindWidget)) UImage* ClueLeftImage;   // 2개용 Left
	UPROPERTY(meta = (BindWidget)) UImage* ClueRightImage;  // 2개용 Right

	// 3개 레이아웃
	UPROPERTY(meta = (BindWidget)) UImage* ClueLeftImage_3;
	UPROPERTY(meta = (BindWidget)) UImage* ClueMidImage_3;
	UPROPERTY(meta = (BindWidget)) UImage* ClueRightImage_3;


	UPROPERTY(meta = (BindWidget)) URichTextBlock* LeftImageItemName;
	UPROPERTY(meta = (BindWidget)) URichTextBlock* RightImageItemName;

	UPROPERTY(meta = (BindWidget)) URichTextBlock* LeftImageItemName_3;
	UPROPERTY(meta = (BindWidget)) URichTextBlock* MidImageItemName_3;
	UPROPERTY(meta = (BindWidget)) URichTextBlock* RightImageItemName_3;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapName;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;
};
