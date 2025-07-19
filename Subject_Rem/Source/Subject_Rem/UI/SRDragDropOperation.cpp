// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRDragDropOperation.h"
#include "UI/SRSlotWidget.h"

void USRDragDropOperation::MoveToSlotData(USRSlotWidget* Drop)
{
	check(DraggedSlot);
	check(Drop);

	if (Drop == DraggedSlot)
		return;

	const FSRItemBaseData& Data = DraggedSlot->GetItemData();
	Drop->SetItemData(Data);
	Drop->SetSlotStyle(Data.Icon);

	DraggedSlot->SetItemData(FSRItemBaseData());
	DraggedSlot->SetSlotStyle(nullptr);
}
