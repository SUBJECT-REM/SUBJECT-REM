// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueWidget.h"
#include "UI/SRSlotWidget.h"
#include "UI/SRClueCombineResultWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"


void USRClueWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

		ClueSlot->Clear();
		ClueSlot->SetSlotButtonNormalStyle(ClueSlotButtonNormalStyle);
		ClueSlot->SetSlotButtonSelectedStyle(ClueSlotButtonSelectedStyle);
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

		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}

	TArray<UWidget*> One_ClueCombineChild = One_ClueCombine->GetAllChildren();

	for (UWidget* Widget : One_ClueCombineChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}


	TArray<UWidget*> Three_ClueCombineGridPanelChild = Three_ClueCombineGridPanel->GetAllChildren();

	for (UWidget* Widget : Three_ClueCombineGridPanelChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}

	
	check(DeviceGridPanel)
		TArray<UWidget*> DeviceGridChild = DeviceGridPanel->GetAllChildren();

	for (UWidget* Widget : DeviceGridChild)
	{
		USRSlotWidget* DeviceSlot = Cast<USRSlotWidget>(Widget);
		if (!DeviceSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(DeviceSlot);
		DeviceSlot->OnSlotClickedDelegate.AddDynamic(this, &ThisClass::OnClickedDeviceSlot);
		DeviceSlot->Clear();
	}
	check(ClueCombineButton)

	ClueCombineButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCombineButton);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void USRClueWidget::UpdateClueGridWidget(const FSRItemBaseData& Data)
{
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		check(ClueSlot);

		if (!ClueSlot->GetIsOccupied())
		{
			ClueSlot->Fill(Data);
			break;
		}
	}
}

void USRClueWidget::UpdateClueCombineResultWidget(const FSRClueMapUIData& Data)
{
	
	ClueCombineResultWidget->SetClueMapImage(Data.ClueIcons);
	ClueCombineResultWidget->SetClueMapDes(Data.ClueMap.Description);
	ClueCombineResultWidget->SetClueMapName(Data.ClueMap.Name);
	if (ClueCombineResultWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		ClueCombineResultWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void USRClueWidget::UpdateDeviceGridWidget(const FSRDeviceUIData& Data)
{
	TArray<UWidget*> Child = DeviceGridPanel->GetAllChildren();
	for (UWidget* Widget : Child)
	{
		USRSlotWidget* DeviceSlot = Cast<USRSlotWidget>(Widget);
		check(DeviceSlot);

		if (!DeviceSlot->GetIsOccupied())
		{
			DeviceSlot->Fill(Data.Base);
			CashedDeviceUsingClueNum.Add(Data.Base.Id, Data.UsingSlotNum);
			break;
		}
	}
}


void USRClueWidget::ClueDataMoveToClueCombine(USRSlotWidget* ClickedSlot)
{
	check(ClueCombineGridPanel)


	TArray<UWidget*> Child = GetCurrentClueCombineGrid()->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueSlot->GetIsOccupied())
		{
			ClueSlot->Fill(ClickedSlot->GetItemData());
			ClickedSlot->Clear();
			ClickedSlot->SetIsEnabled(true);
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

			ClueSlot->Fill(ClickedSlot->GetItemData());
			ClickedSlot->Clear();
			ClickedSlot->SetIsEnabled(true);

			break;
		}

	}
	
}

void USRClueWidget::OnClickedCombineButton()
{
	check(ClueCombineGridPanel)

	TArray<FName> CombinedClueIds;

	for (UWidget* Widget : GetCurrentClueCombineGrid()->GetAllChildren())
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		FSRItemBaseData Data = ClueCombineSlot->GetItemData();
		if (!Data.Id.IsNone())
		{
			CombinedClueIds.Add(Data.Id);
		}

		//조합 이후 ClueCombinePanel 아이템들 제거
		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}

	if (CombinedClueIds.Num() == CurVaildCombineItemNum)
	{
		CombineButtonClickedDelegate.Broadcast(CombinedClueIds);
		
		//사용한 디바이스 삭제 
		if (CurUsingDevicedSlot)
		{
			RemoveDeviceDataDelegate.Broadcast(CurUsingDevicedSlot->GetItemData().Id);
			CashedDeviceUsingClueNum.Remove(CurUsingDevicedSlot->GetItemData().Id);
			CurUsingDevicedSlot->Clear();
			CurUsingDevicedSlot = nullptr;
			ClueCombineSwitcher->SetActiveWidgetIndex(1); //1 default 
		}
	}


}

void USRClueWidget::OnClickedDeviceSlot(USRSlotWidget* ClickedSlot)
{
	//ClickedSlot->GetItemData().Id
	if (!ClickedSlot->GetItemData().Id.IsNone())
	{
		FName DeviceId = ClickedSlot->GetItemData().Id;
		UE_LOG(LogTemp, Warning, TEXT("Deviced Clicked Id : %s"), *DeviceId.ToString());
		uint8* FindUsingCombineSlotNumPtr = CashedDeviceUsingClueNum.Find(DeviceId);
		if (!FindUsingCombineSlotNumPtr)
			return;
		uint8 FindUsingCombineSlotNum = *FindUsingCombineSlotNumPtr;
		ClueCombineSwitcher->SetActiveWidgetIndex(FindUsingCombineSlotNum - 1);
		CurVaildCombineItemNum = FindUsingCombineSlotNum;
		CurUsingDevicedSlot = ClickedSlot;
		
	}
}

UGridPanel* USRClueWidget::GetCurrentClueCombineGrid()
{
	return Cast<UGridPanel>(ClueCombineSwitcher->GetActiveWidget());
}
