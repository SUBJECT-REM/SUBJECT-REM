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
				UE_LOG(LogTemp, Warning, TEXT("QuickSlot Widget Bind To Player QuickSlotComp"));
				QuickSlotComp->OnQuickSlotChangedDelegate.AddDynamic(this, &ThisClass::OnSlotChange);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuickSlot Widget PlayerPawn nullptr"));
	}
	
}

void USRQuickSlotWidget::OnSlotChange(int32 Index, TSoftObjectPtr<UTexture2D> Icon)
{
	UWidget* Child = QuickSlotGridPanel->GetChildAt(Index);

	USRSlotWidget* QuickSlot = Cast<USRSlotWidget>(Child);

	QuickSlot->SetItemIcon(Icon);
}
