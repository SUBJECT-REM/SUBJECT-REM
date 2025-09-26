// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlot/SRQuickSlotWidget.h"
#include "UI/SRSlotWidget.h"
#include "GameFramework/Pawn.h"
#include "Component/Character/SRQuickSlotComponent.h"
#include "Components/GridPanel.h"
#include "SRItem.h"


void USRQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		AActor* Actor = Cast<AActor>(Pawn);
		if (Actor)
		{
			USRQuickSlotComponent* QuickSlotComp = Actor->FindComponentByClass<USRQuickSlotComponent>();
			if (QuickSlotComp)
			{
				QuickSlotComp->OnQuickSlotChangedDelegate.AddDynamic(this, &ThisClass::OnSlotChange);
			}
		}
	}

	TArray<UWidget*> GridChild = QuickSlotGridPanel->GetAllChildren();

	for (UWidget* Child : GridChild)
	{
		USRSlotWidget* SlotWidget = Cast<USRSlotWidget>(Child);

		if (SlotWidget)
		{
			SlotWidget->SetItemIconVisualOnly(nullptr);
		}
	}
}

void USRQuickSlotWidget::OnSlotChange(int32 Index, TSoftObjectPtr<UTexture2D> Icon)
{
	UWidget* Child = QuickSlotGridPanel->GetChildAt(Index);

	USRSlotWidget* QuickSlot = Cast<USRSlotWidget>(Child);

	QuickSlot->SetItemIconVisualOnly(Icon);
}
