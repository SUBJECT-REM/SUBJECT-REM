// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueWidget.h"
#include "UI/SRSlotWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"

void USRClueWidget::NativeConstruct()
{
	check(ClueGridPanel)

	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueSlot);

		//버튼 비활성화
		ClueSlot->SetIsEnabled(false);
		ClueSlot->FOnSlotClickedDelegate.AddDynamic(this, &ThisClass::ClueDataMoveToClueCombine);
	}
	

	check(ClueCombineGridPanel)

	TArray<UWidget*> Child = ClueCombineGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		//버튼 비활성화
		ClueCombineSlot->SetIsEnabled(false);
		ClueCombineSlot->FOnSlotClickedDelegate.AddDynamic(this, &ThisClass::ClueCombineDataMoveToClue);
	}
	
}

void USRClueWidget::UpdateClueWidget(const FSRClueData& Data)
{
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		if (!Widget->GetIsEnabled())
		{
			USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
			if (!ClueSlot)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClueGridPanel Children Cast cannot be cast to USRSlotWidget"));
			}
			check(ClueSlot);

			ClueSlot->SetClueData(Data);
			ClueSlot->SetSlotStyle(ClueSlot->GetClueData().Icon);
			
			//버튼을 활성화 합니다.
			ClueSlot->SetIsEnabled(true);
			break;
		}
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
			ClueSlot->SetClueData(ClickedSlot->GetClueData());
			ClueSlot->SetSlotStyle(ClueSlot->GetClueData().Icon);
			ClickedSlot->SetSlotStyle(nullptr);

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
			ClueSlot->SetClueData(ClickedSlot->GetClueData());
			ClueSlot->SetSlotStyle(ClueSlot->GetClueData().Icon);
			ClickedSlot->SetSlotStyle(nullptr);

			ClueSlot->SetIsEnabled(true);
			ClickedSlot->SetIsEnabled(false);
			break;
		}

	}
	
}
