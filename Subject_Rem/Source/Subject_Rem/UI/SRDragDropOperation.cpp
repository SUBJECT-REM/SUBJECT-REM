// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRDragDropOperation.h"
#include "UI/SRSlotWidget.h"

void USRDragDropOperation::MoveToSlotData(USRSlotWidget* Drop)
{
	check(DraggedSlot);
	check(Drop);
	
	//Drop 슬롯이 이미 사용중이라면
	if (Drop->GetIsOccupied())
	{
		UE_LOG(LogTemp, Warning, TEXT("DropSlot Is Occuiped"));
		return;
	}

	Drop->Fill(DraggedSlotItemData);

	//드래그한곳에서 다시 드랍한다면
	if (Drop == DraggedSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop Slot equal DrggedSlot "));
		return;
	}

	DraggedSlot->Clear();
	UE_LOG(LogTemp, Warning, TEXT("SlotDragDropOper : Move To Data "));
}
