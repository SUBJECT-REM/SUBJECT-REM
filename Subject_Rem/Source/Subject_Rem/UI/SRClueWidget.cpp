﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueWidget.h"
#include "UI/SRSlotWidget.h"
#include "UI/SRClueCombineResultWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridPanel.h"


void USRClueWidget::NativeConstruct()
{
	check(ClueGridPanel)
	TArray<UWidget*> ClueGridChild = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : ClueGridChild)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueSlot);

		ClueSlot->SetIsEnabled(true);
	}
	

	check(ClueCombineGridPanel)
	TArray<UWidget*> ClueCombineChild = ClueCombineGridPanel->GetAllChildren();

	for (UWidget* Widget : ClueCombineChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		ClueCombineSlot->SetIsEnabled(true);
	}
	
	check(ClueCombineButton)
	ClueCombineButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCombineButton);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void USRClueWidget::UpdateClueGridWidget(const FSRItemBaseData& Data)
{
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();
	UE_LOG(LogTemp, Warning, TEXT("UpdateClueWidget"));

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		check(ClueSlot);

		if (ClueSlot->GetItemData().Icon == nullptr)
		{
			if (!ClueSlot)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClueGridPanel Children Cast cannot be cast to USRSlotWidget"));
			}

			UE_LOG(LogTemp, Warning, TEXT("UpdateClueWidget for "));

			ClueSlot->SetItemData(Data);
			ClueSlot->SetSlotIcon(ClueSlot->GetItemData().Icon);

			//버튼을 활성화 합니다.
			ClueSlot->SetIsOccupied(true);

			break;
		}
	}
}

void USRClueWidget::UpdateClueCombineResultWidget(const FSRItemBaseData& Data)
{
	ClueCombineResultWidget->SetClueMapImage(Data.Icon);
	ClueCombineResultWidget->SetClueMapDes(Data.Description);
	ClueCombineResultWidget->SetClueMapName(Data.Name);
	if (ClueCombineResultWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		ClueCombineResultWidget->SetVisibility(ESlateVisibility::Visible);
	}

}


void USRClueWidget::ClueDataMoveToClueCombine(USRSlotWidget* ClickedSlot)
{
	check(ClueCombineGridPanel)

	TArray<UWidget*> Child = ClueCombineGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		if (!Widget->GetIsEnabled())
		{
			USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
			ClueSlot->SetItemData(ClickedSlot->GetItemData());
			ClueSlot->SetSlotIcon(ClueSlot->GetItemData().Icon);
			ClickedSlot->SetSlotIcon(nullptr);

			//버튼을 활성화 합니다.
			ClueSlot->SetIsEnabled(true);
			ClickedSlot->SetIsEnabled(false);
			break;
		}
			
	}
	
}

void USRClueWidget::ClueCombineDataMoveToClue(USRSlotWidget* ClickedSlot)
{
	check(ClueGridPanel)
	
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		if (!Widget->GetIsEnabled())
		{
			USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
			ClueSlot->SetItemData(ClickedSlot->GetItemData());
			ClueSlot->SetSlotIcon(ClueSlot->GetItemData().Icon);
			ClickedSlot->SetSlotIcon(nullptr);

			ClueSlot->SetIsEnabled(true);
			ClickedSlot->SetIsEnabled(false);
			break;
		}

	}
	
}

void USRClueWidget::OnClickedCombineButton()
{
	check(ClueCombineGridPanel)

	TArray<FName> CombinedClueIds;

	for (UWidget* Widget : ClueCombineGridPanel->GetAllChildren())
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		FSRItemBaseData Data = ClueSlot->GetItemData();
		if (!Data.Id.IsNone())
		{
			CombinedClueIds.Add(Data.Id);
		}

		//조합 이후 ClueCombinePanel 아이템들 제거
		ClueSlot->SetSlotIcon(nullptr);
		ClueSlot->SetItemData(FSRItemBaseData());
		ClueSlot->SetIsEnabled(false);
	}

	if (CombinedClueIds.Num() == VaildCombineItemNum)
	{
		CombineButtonClickedDelegate.Broadcast(CombinedClueIds);
	}


}
