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
class USRRotateItemPreviewWidget;

USTRUCT()
struct FSlotStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> SlotImageNormal;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> SlotImageHover;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> SlotImageClicked;
};

UCLASS()
class SUBJECT_REM_API USRInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemName(FName Name);
	void UpdateItemDes(FName Des);
	void UpdateItemPreview(TSoftObjectPtr<UStaticMesh> Mesh);

	void AddItemInventoryGridPanel(const FSRItemBaseData& Data);
	void RemoveItemInventoryGridPanel(const TArray<FName>& ItemIds);

	void UpdateQuickSlotGridPanel(int8 Index, TSoftObjectPtr<UTexture2D> Icon);
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

	UFUNCTION()
	void RegisterItemInQuickSlot(USRSlotWidget* DropedSlot, USRSlotWidget* DraggedSlot);

	UFUNCTION()
	void UnRegisterItemInQuickSlot(USRSlotWidget* DropedSlot, USRSlotWidget* DraggedSlot);
	/*아이템 이름, 설명을 포함하는 박스패널*/
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemInfoTextBox;

	/*아이템 이름란*/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	/*아이템 설명란*/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	/*아이템 Preview*/
	UPROPERTY(meta = (BindWidget))
	USRRotateItemPreviewWidget* ItemPreview;


	UPROPERTY(meta = (BindWidget))
	UGridPanel* QuickSlotGridPanel;

	UPROPERTY(EditDefaultsOnly)
	FSlotStyle InventorySlotImage;

	UPROPERTY(EditDefaultsOnly)
	FSlotStyle QuickSlotImage;

	//UPROPERTY(EditDefaultsOnly)
	//TSoftObjectPtr<UTexture2D> InventorySlotImageClicked;



};
