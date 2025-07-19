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
		InvenSlot->FOnSlotClickedDelegate.AddDynamic(this, &ThisClass::UpdateItemDescriptionPanel);
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

void USRInventoryWidget::UpdateInventoryGridPanel(const FSRItemBaseData& Data)
{
	TArray<UWidget*> Child = InventoryGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		check(InvenSlot);

		if (InvenSlot->GetItemData().Icon == nullptr)
		{

			InvenSlot->SetItemData(Data);
			InvenSlot->SetSlotStyle(InvenSlot->GetItemData().Icon);

			//버튼을 활성화 합니다.
			InvenSlot->SetIsEnabled(true);
			break;
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
