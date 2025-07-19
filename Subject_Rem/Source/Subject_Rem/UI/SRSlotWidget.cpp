// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/SRDragDropOperation.h"
#include "Components/Button.h"

void USRSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnPressed.AddDynamic(this, &ThisClass::OnButtonClicked);
	}

	DefaultSlotStyle = Button->GetStyle();
}

void USRSlotWidget::SetSlotStyle(UObject* Icon)
{
	check(Button)

	if (Icon == nullptr)
	{
		Button->SetStyle(DefaultSlotStyle);
	}

	else
	{
		FButtonStyle ButtonStyle;
		FSlateBrush NormalStyle;
		NormalStyle.SetResourceObject(Icon);
		ButtonStyle.SetNormal(NormalStyle);

		FSlateBrush HoverStyle;
		HoverStyle.SetResourceObject(Icon);
		ButtonStyle.SetHovered(HoverStyle);

		FSlateBrush PressStyle;
		PressStyle.SetResourceObject(Icon);
		ButtonStyle.SetPressed(PressStyle);

		Button->SetStyle(ButtonStyle);
	}
}

FButtonStyle USRSlotWidget::GetSlotStyle()
{
	return Button->GetStyle();
}

void USRSlotWidget::SetItemData(const FSRItemBaseData& NewData)
{
	ItemData = NewData;
	bIsOccupied = true;
}

const FSRItemBaseData& USRSlotWidget::GetItemData() const
{
	return ItemData;
}

void USRSlotWidget::OnButtonClicked()
{
	FOnSlotClickedDelegate.Broadcast(this);
}

FReply USRSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}


	return Reply.NativeReply;
}

void USRSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		USRDragDropOperation* DragDropOper = NewObject<USRDragDropOperation>();
	
		check(DragDropOper);
		OutOperation = DragDropOper;
		DragDropOper->DefaultDragVisual = this;
		DragDropOper->DraggedSlot = this;
	}

}

bool USRSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation)
	{
		USRDragDropOperation* DragDropOper = Cast<USRDragDropOperation>(InOperation);

		check(DragDropOper);

		DragDropOper->MoveToSlotData(this);

		//ItemData = DragDropOper->DraggedSlot->ItemData;
		//DragDropOper->DraggedSlot->SetSlotStyle(nullptr);
		//DragDropOper->DraggedSlot->ItemData = FSRItemBaseData();
		//SetSlotStyle(ItemData.Icon);
	}

	return true;
}	
