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
	
}

void USRQuickSlotWidget::OnSlotChange(int32 Index)
{
	USRQuickSlotComponent* QuickSlotComp = nullptr;
	UWidget* Child = QuickSlotGridPanel->GetChildAt(Index);

	USRSlotWidget* QuickSlot = Cast<USRSlotWidget>(Child);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		AActor* Actor = Cast<AActor>(Pawn);
		if (Actor)
		{
			QuickSlotComp = Actor->FindComponentByClass<USRQuickSlotComponent>();
		}
	}

	//Index에 해당하는 ID를 받아옴
	if (QuickSlotComp)
	{
		//QuickSlotComp->
	}

	//DataTable에서 ID에 따른 Image , ... 받아옴
	

	//갱신

}
