// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemData.h"
#include "SRSlotWidget.generated.h"

/**
 * 
 */

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClickedSignature, USRSlotWidget*, ClickedSlot);

UCLASS()
class SUBJECT_REM_API USRSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnSlotClickedSignature FOnSlotClickedDelegate;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	/*
	* 버튼 이미지를 지정합니다.
	* @param 버튼 이미지 텍스처
	*/
	void SetSlotStyle(UObject* Icon);
	FButtonStyle GetSlotStyle();

	void SetItemData(const FSRItemBaseData& NewData);
	const FSRItemBaseData& GetItemData() const;



	bool IsEmpty() const { return !bIsOccupied; }
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnButtonClicked();

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	//메시, 아이콘, 이름 ,Id, 설명이 담겨있습니다.
	FSRItemBaseData ItemData;

	bool bIsOccupied = false;
	
	FButtonStyle DefaultSlotStyle;
};
