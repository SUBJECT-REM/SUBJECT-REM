// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRInventoryWidget.h"
#include "UI/SRSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"
#include "Component/Character/SRQuickSlotComponent.h"



void USRInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(InventoryGridPanel)
	TArray<UWidget*> InventoryGridChild = InventoryGridPanel->GetAllChildren();

	for (UWidget* Widget : InventoryGridChild)
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		if (!InvenSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("InvenGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		InvenSlot->SetSlotButtonNormalStyle(InventorySlotButtonNormalStyle);
		InvenSlot->SetSlotButtonSelectedStyle(InventorySlotButtonSelectedStyle);

		InvenSlot->SetIsEnabled(true);
		InvenSlot->OnSlotClickedDelegate.AddDynamic(this, &ThisClass::UpdateItemDescriptionPanel);
		InvenSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::UnRegisterItemInQuickSlot);
	}

	check(QuickSlotGridPanel)
	TArray<UWidget*> QuickSlotGridChild = QuickSlotGridPanel->GetAllChildren();

	for (UWidget* Widget : QuickSlotGridChild)
	{
		USRSlotWidget* QuickSlot = Cast<USRSlotWidget>(Widget);
		if (!QuickSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("QuickSlotGridChild Children Cast cannot be cast to USRSlotWidget"));
		}
		QuickSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::RegisterItemInQuickSlot);
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
			InvenSlot->SetItemIcon(InvenSlot->GetItemData().Icon);
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
			InvenSlot->SetItemIcon(nullptr);
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

void USRInventoryWidget::RegisterItemInQuickSlot(USRSlotWidget* DropedSlot, USRSlotWidget* DraggedSlot)
{
	USRQuickSlotComponent* QuickSlotComp = nullptr;

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		AActor* Actor = Cast<AActor>(Pawn);
		if (Actor)
		{
			QuickSlotComp= Actor->FindComponentByClass<USRQuickSlotComponent>();
		}
	}

	if (!QuickSlotComp)
	{
		return;
	}
	if (DraggedSlot->IsChildOf(InventoryGridPanel) && DropedSlot->IsChildOf(QuickSlotGridPanel))
	{
		int32 Index = QuickSlotGridPanel->GetChildIndex(DropedSlot);

		UE_LOG(LogTemp, Warning, TEXT("RegistItem Requset to quick slot %d, %s"),Index, *DropedSlot->GetItemData().Id.ToString());

		QuickSlotComp->RegisterItem(Index, DropedSlot->GetItemData().Id);
	}
}

void USRInventoryWidget::UnRegisterItemInQuickSlot(USRSlotWidget* DropedSlot, USRSlotWidget* DraggedSlot)
{
	USRQuickSlotComponent* QuickSlotComp = nullptr;

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		AActor* Actor = Cast<AActor>(Pawn);
		if (Actor)
		{
			QuickSlotComp = Actor->FindComponentByClass<USRQuickSlotComponent>();
		}
	}

	if (DraggedSlot->IsChildOf(QuickSlotGridPanel) && DropedSlot->IsChildOf(InventoryGridPanel))
	{
		int32 Index = QuickSlotGridPanel->GetChildIndex(DraggedSlot);

		QuickSlotComp->UnRegisterItem(Index);
	}
}
