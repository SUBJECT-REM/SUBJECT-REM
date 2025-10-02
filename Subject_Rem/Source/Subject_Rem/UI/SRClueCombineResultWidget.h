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
	UFUNCTION(BlueprintCallable)
	void SetClueMapImage(TArray<TSoftObjectPtr<UTexture2D>> Icons);

	UFUNCTION(BlueprintCallable)
	void SetClueMapDes(FText Text);

	UFUNCTION(BlueprintCallable)
	void SetClueMapName(FText Text);

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* CloseButton;
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickedCloseButton();

	void EnsureLayoutByCount(uint8 Count);

	void SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D>& SoftTex);


	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* LayoutSwitcher;

	//1ro
	UPROPERTY(meta = (BindWidget))
	UImage* ClueImage;

	//2개 레이아웃
	UPROPERTY(meta = (BindWidget))
	UImage* ClueLeftImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ClueRightImage;

	// 3개 레이아웃
	UPROPERTY(meta = (BindWidget)) UImage* ClueLeftImage_3;
	UPROPERTY(meta = (BindWidget)) UImage* ClueMidImage_3;
	UPROPERTY(meta = (BindWidget)) UImage* ClueRightImage_3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapName;


};
