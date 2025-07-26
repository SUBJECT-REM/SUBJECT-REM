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
		DefaultSlotStyle = Button->GetStyle();
	}

}

FReply USRSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!bIsOccupied)
	{
		return reply.NativeReply;
	}

	OnSlotClickedDelegate.Broadcast(this);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}


	return reply.NativeReply;
}

void USRSlotWidget::SetSlotIcon(UObject* Icon)
{
	check(Button)

		if (Icon == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Icon nullptr"));	
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

void USRSlotWidget::SetItemData(const FSRItemBaseData& NewData)
{
	ItemData = NewData;
}

const FSRItemBaseData& USRSlotWidget::GetItemData() const
{
	return ItemData;
}

void USRSlotWidget::SetIsOccupied(bool IsOccupied)
{
	bIsOccupied = IsOccupied;
}

void USRSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	check(DragVisualWidgetClass);

	if (OutOperation == nullptr)
	{
		USRDragDropOperation* DragDropOper = NewObject<USRDragDropOperation>();
		USRSlotWidget* DragPreview = CreateWidget<USRSlotWidget>(GetWorld(), DragVisualWidgetClass); // 드래그 미리보기 위젯 클래스

		check(DragDropOper);
		check(DragPreview);

		DragDropOper->OnDragCancelled.AddDynamic(this, &ThisClass::SlotDragCancelled);

		DragPreview->SetSlotIcon(ItemData.Icon);

		OutOperation = DragDropOper;
		DragDropOper->DefaultDragVisual = DragPreview;
		DragDropOper->DraggedSlot = this;
		DragDropOper->DraggedSlotItemData = ItemData;

		//빈슬롯으로 세팅해줍니다.
		SetIsOccupied(false);
		SetSlotIcon(nullptr);
		SetItemData(FSRItemBaseData());
	}

}

bool USRSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//이미 사용중인 슬롯이라면 Drop하지 하지 않도록 합니다.
	if (GetIsOccupied())
		return false;

	if (InOperation)
	{
		USRDragDropOperation* DragDropOper = Cast<USRDragDropOperation>(InOperation);

		check(DragDropOper);

		DragDropOper->MoveToSlotData(this);
	}

	return true;
}

void USRSlotWidget::SlotDragCancelled(UDragDropOperation* DragDropOper)
{
	USRDragDropOperation* SlotDragDropOper = Cast<USRDragDropOperation>(DragDropOper);
	USRSlotWidget* DraggedSlot = SlotDragDropOper->DraggedSlot;

	check(SlotDragDropOper);
	check(DraggedSlot);

	const FSRItemBaseData SlotItemData = SlotDragDropOper->DraggedSlotItemData;

	SetItemData(SlotItemData);
	SetSlotIcon(SlotItemData.Icon);
	SetIsOccupied(true);
}

