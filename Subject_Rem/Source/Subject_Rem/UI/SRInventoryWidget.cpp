// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRInventoryWidget.h"
#include "UI/SRSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"



void USRInventoryWidget::NativeConstruct()
{
	check(InventoryGridPanel)
		TArray<UWidget*> InventoryGridChild = InventoryGridPanel->GetAllChildren();

	for (UWidget* Widget : InventoryGridChild)
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		if (!InvenSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("InvenGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(InvenSlot);

		InvenSlot->SetIsEnabled(true);
		InvenSlot->OnSlotClickedDelegate.AddDynamic(this, &ThisClass::UpdateItemDescriptionPanel);
	}

	ItemName->SetText(FText::GetEmpty());
	ItemDescription->SetText(FText::GetEmpty());
	ItemInfoTextBox->SetVisibility(ESlateVisibility::Hidden);
}

void USRInventoryWidget::UpdateItemName(FName Name)
{
	ItemName->SetText(FText::FromName(Name));
}

void USRInventoryWidget::UpdateItemDes(FName Des)
{
	ItemDescription->SetText(FText::FromName(Des));
}

void USRInventoryWidget::AddItemInventoryGridPanel(const FSRItemBaseData& Data)
{
	TArray<UWidget*> Child = InventoryGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		check(InvenSlot);

		if (!InvenSlot->GetIsOccupied())
		{
			UE_LOG(LogTemp, Warning, TEXT("Updated Inventory Slot Name %s"), *InvenSlot->GetName());

			InvenSlot->SetItemData(Data);
			InvenSlot->SetSlotIcon(InvenSlot->GetItemData().Icon);
			InvenSlot->SetIsOccupied(true);
			break;
		}
	}
}

void USRInventoryWidget::RemoveItemInventoryGridPanel(const TArray<FName>& ItemIds)
{
	for (UWidget* Widget : InventoryGridPanel->GetAllChildren())
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		if (!InvenSlot) continue;

		const FSRItemBaseData& Data = InvenSlot->GetItemData();
		if (ItemIds.Contains(Data.Id))
		{
			InvenSlot->SetSlotIcon(nullptr);
			InvenSlot->SetItemData(FSRItemBaseData());
			InvenSlot->SetIsOccupied(false);
		}
	}
}

void USRInventoryWidget::UpdateItemDescriptionPanel(USRSlotWidget* ClickedSlot)
{
	const FSRItemBaseData& Data = ClickedSlot->GetItemData();
	ItemInfoTextBox->SetVisibility(ESlateVisibility::Visible);
	//Update항목들
	UpdateItemName(Data.Name);
	UpdateItemDes(Data.Description);
}
