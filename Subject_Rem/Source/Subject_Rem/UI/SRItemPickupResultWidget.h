// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemPickupResultWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

UCLASS()
class SUBJECT_REM_API USRItemPickupResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetItemDes(FName Text);
	void SetItemName(FName Text);
private:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void HandleVisibilityChange(ESlateVisibility NewVisibility);

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDes;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

};
