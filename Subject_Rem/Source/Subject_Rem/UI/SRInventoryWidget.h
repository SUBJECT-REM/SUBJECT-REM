// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemData.h"
#include "SRInventoryWidget.generated.h"

/**
 * 
 */
class UGridPanel;
class UTextBlock;
class USRSlotWidget;
class UVerticalBox;
class UUniformGridPanel;

UCLASS()
class SUBJECT_REM_API USRInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemName(FName Name);
	void UpdateItemDes(FName Des);

	void AddItemInventoryGridPanel(const FSRItemBaseData& Data);
	void RemoveItemInventoryGridPanel(const TArray<FName>& ItemIds);
protected:
	virtual void NativeConstruct() override;


	/** 인벤토리 슬롯용 버튼 스타일 (에디터에서 세팅) */
	UPROPERTY(EditDefaultsOnly, Category = "Styles" ,BlueprintReadOnly)
	FButtonStyle InventorySlotButtonNormalStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles", BlueprintReadOnly)
	FButtonStyle InventorySlotButtonSelectedStyle;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* InventoryGridPanel;

private:
	UFUNCTION()
	void UpdateItemDescriptionPanel(USRSlotWidget* ClickedSlot);
	/*아이템들을 보여주는 패널*/


	/*아이템 이름, 설명을 포함하는 박스패널*/
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemInfoTextBox;

	/*아이템 이름란*/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	/*아이템 설명란*/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* asdsa;
};
