// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueCombineWidget.h"
#include "UI/SRSlotWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"


void USRClueCombineWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClueCombineSwitcher->SetActiveWidget(TwoPanel);
	CurrentActivePanel = TwoPanel;

	TArray<UWidget*> OnePanelChild = OnePanel->GetAllChildren();

	for (UWidget* Widget : OnePanelChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		ClueCombineSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::OnSlotDropped);
		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}

	TArray<UWidget*> TwoPanelChild = TwoPanel->GetAllChildren();

	for (UWidget* Widget : TwoPanelChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);

		ClueCombineSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::OnSlotDropped);
		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}


	TArray<UWidget*> ThreePanelChild = ThreePanel->GetAllChildren();

	for (UWidget* Widget : ThreePanelChild)
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueCombineSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueCombineSlot);
		ClueCombineSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::OnSlotDropped);
		ClueCombineSlot->Clear();
		ClueCombineSlot->SetIsEnabled(true);
	}
}

void USRClueCombineWidget::SetLayoutByCount(int32 Count)
{
	ClueCombineSwitcher->SetActiveWidgetIndex(Count);

	switch (Count)
	{
	case 0:
		CurrentActivePanel = OnePanel;
		break;
	case 1:
		CurrentActivePanel = TwoPanel;
		break;
	case 2:
		CurrentActivePanel = ThreePanel;
	}
}

UPanelWidget* USRClueCombineWidget::GetActivePanelWidget()
{
	return CurrentActivePanel;
}

void USRClueCombineWidget::PutBackDevice()
{
	if (CurrentActivePanel == ThreePanel)
	{
		TArray<UWidget*> WidgetChildren =ThreePanel->GetAllChildren();
		for (UWidget* Child : WidgetChildren)
		{
			USRSlotWidget* SlotWidget = Cast<USRSlotWidget>(Child);
			if (SlotWidget)
			{
				HandleOptionalResource(SlotWidget, nullptr);
			}
		}
	}
}

void USRClueCombineWidget::HandleOptionalResource(USRSlotWidget* DroppedSlot, TSoftObjectPtr<UTexture2D> Resource)
{
	int32 Index = ThreePanel->GetChildIndex(DroppedSlot);
	if (UWidget* Widget = ThreeOptionalPanel->GetChildAt(Index))
	{
		UImage* Image = Cast<UImage>(Widget);
		if (Image)
		{
			if (!Resource.IsNull())
			{
				Image->SetBrushFromSoftTexture(Resource);
				Image->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
			else
			{
				Image->SetBrushFromSoftTexture(nullptr);
				Image->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.f));
			}
		}
	}
}


void USRClueCombineWidget::OnSlotDropped(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot)
{
	OnCombineSlotDropped.Broadcast(DroppedSlot, DraggedSlot);
}
