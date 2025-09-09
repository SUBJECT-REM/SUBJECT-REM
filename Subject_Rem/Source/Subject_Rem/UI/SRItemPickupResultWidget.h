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
class USRRotateItemPreviewWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupResultClosed);

UCLASS()
class SUBJECT_REM_API USRItemPickupResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetItemPreview(TSoftObjectPtr<UStaticMesh> Mesh);
	void SetItemDes(FText Text);
	void SetItemName(FName Text);

	FPickupResultClosed OnClosedDelegate;
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	USRRotateItemPreviewWidget* ItemPreview;
private:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void HandleVisibilityChange(ESlateVisibility NewVisibility);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDes;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "ItemPreview")
	float ItemPreviewWidgetHeight;

	UPROPERTY(EditDefaultsOnly, Category = "ItemPreview")
	float ItemPreviewWidgetWidth;
};
