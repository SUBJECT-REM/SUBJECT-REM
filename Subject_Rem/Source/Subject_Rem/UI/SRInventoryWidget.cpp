// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRInventoryWidget.h"
#include "UI/SRSlotWidget.h"
#include "UI/SRRotateItemPreviewWidget.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Component/Character/SRQuickSlotComponent.h"
#include "Components/RichTextBlock.h"


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
		InvenSlot->Clear();
		InvenSlot->SetSlotButtonNormalStyle(InventorySlotButtonNormalStyle);
		InvenSlot->SetSlotButtonSelectedStyle(InventorySlotButtonSelectedStyle);

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
		QuickSlot->SetItemIconVisualOnly(nullptr);
		QuickSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::RegisterItemInQuickSlot);
	}


	ItemObserveImage->SetVisibility(ESlateVisibility::Hidden);
	ItemName->SetText(FText::GetEmpty());
	ItemDescription->SetText(FText::GetEmpty());
	ItemInfoTextBox->SetVisibility(ESlateVisibility::Hidden);

}

void USRInventoryWidget::UpdateItemName(FText Name)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateItemName : %s "), *Name.ToString());
	ItemName->SetText(Name);
	ItemName->InvalidateLayoutAndVolatility();
	ItemName->ForceLayoutPrepass();
}

void USRInventoryWidget::UpdateItemDes(FText Des)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateItemDes : %s "), *Des.ToString());
	ItemDescription->SetText(Des);
	ItemDescription->InvalidateLayoutAndVolatility();
	ItemDescription->ForceLayoutPrepass();
}

void USRInventoryWidget::UpdateItemPreview(TSoftObjectPtr<UStaticMesh> Mesh)
{
	ItemPreview->SetItemPreviewMesh(Mesh);
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

			InvenSlot->Fill(Data);
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
			InvenSlot->Clear();
		}
	}

	TArray<const FSRItemBaseData> Datas;
	for (UWidget* Widget : InventoryGridPanel->GetAllChildren())
	{
		USRSlotWidget* InvenSlot = Cast<USRSlotWidget>(Widget);
		if (!InvenSlot) continue;

		if (InvenSlot->GetIsOccupied())
		{
			Datas.Add(InvenSlot->GetItemData());
			InvenSlot->Clear();
		}
	}

	//다시 채워서 재정렬 
	for (const FSRItemBaseData& Data : Datas)
	{
		AddItemInventoryGridPanel(Data);
	}



}

void USRInventoryWidget::UpdateQuickSlotGridPanel(int8 Index, TSoftObjectPtr<UTexture2D> Icon)
{
	UWidget* FindWidget = QuickSlotGridPanel->GetChildAt(Index);
	if (!FindWidget)
		return;

	USRSlotWidget* QuickSlot = Cast<USRSlotWidget>(FindWidget);
	if (!QuickSlot)
		return;

	QuickSlot->SetIsEnabled(true);
	QuickSlot->SetItemIconVisualOnly(Icon);
}

void USRInventoryWidget::UpdateItemDescriptionPanel(USRSlotWidget* ClickedSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked Slot NAme :%s "), *ClickedSlot->GetName());
	const FSRItemBaseData& Data = ClickedSlot->GetItemData();
	ItemObserveImage->SetVisibility(ESlateVisibility::Visible);
	ItemInfoTextBox->SetVisibility(ESlateVisibility::Visible);
	//Update항목들
	UpdateItemName(Data.Name);
	UpdateItemDes(Data.Description);
	UpdateItemPreview(Data.Mesh);
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
		const int32 Index = QuickSlotGridPanel->GetChildIndex(DropedSlot);
		const FSRItemBaseData MovedData = DropedSlot->GetItemData(); // 지금 드랍된(옮겨진) 데이터

		// 1) 등록 시도
		const bool bOk = QuickSlotComp->RegisterItem(Index, MovedData.Id);

		if (!bOk)
		{
			// 2) 실패 → 시각적으로 되돌리기
			//// (a) 드랍된 퀵슬롯 비우기
			//DropedSlot->SetItemIcon(nullptr);
			//DropedSlot->SetItemData(FSRItemBaseData());
			//DropedSlot->SetIsOccupied(false);
			DropedSlot->Clear();

			// (b) 원래 인벤토리 슬롯 복구
			DraggedSlot->Fill(MovedData);/*
			DraggedSlot->SetItemData(MovedData);
			DraggedSlot->SetItemIcon(MovedData.Icon);
			DraggedSlot->SetIsOccupied(true);*/

			UE_LOG(LogTemp, Warning, TEXT("QuickSlot register rejected. Reverted visuals."));
			return;
		}

		// 성공 시엔 그대로 유지 (QuickSlotComp가 OnQuickSlotChanged로 위젯 갱신함)
		UE_LOG(LogTemp, Warning, TEXT("RegistItem to quick slot %d, %s"), Index, *MovedData.Id.ToString());
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
