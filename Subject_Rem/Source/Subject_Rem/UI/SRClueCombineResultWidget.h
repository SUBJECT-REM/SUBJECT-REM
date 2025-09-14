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
	void SetClueMapImage(TSoftObjectPtr<UTexture2D> LeftIcon, TSoftObjectPtr<UTexture2D> RightIcon);

	UFUNCTION(BlueprintCallable)
	void SetClueMapDes(FText Text);

	UFUNCTION(BlueprintCallable)
	void SetClueMapName(FName Text);

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* CloseButton;
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickedCloseButton();

	UPROPERTY(meta = (BindWidget))
	UImage* ClueLeftImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ClueRightImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapDes;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClueMapName;


};
