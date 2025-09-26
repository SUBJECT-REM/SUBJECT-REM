// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include"UI/SRSlotDragVisualWidget.h"
#include "UI/SRDragDropOperation.h"
#include "Components/Button.h"
#include "Components/Image.h"


void USRSlotWidget::SetItemIconVisualOnly(TSoftObjectPtr<UTexture2D> IconTexture)
{
	if (!ItemIconImage)
		return;

	if (!IconTexture.IsNull())
	{
		ItemIconImage->SetBrushFromSoftTexture(IconTexture, true);
		ItemIconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ItemIconImage->SetBrushFromSoftTexture(nullptr, true);
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USRSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

void USRSlotWidget::Fill(const FSRItemBaseData& NewData)
{
	SetItemData(NewData);
	SetItemIcon(NewData.Icon);
	SetIsOccupied(true);

}

void USRSlotWidget::Clear()
{
	SetItemIcon(nullptr);
	SetItemData(FSRItemBaseData());
	SetIsOccupied(false);
}

void USRSlotWidget::SetItemIcon(TSoftObjectPtr<UTexture2D> IconTexture)
{
	if (!ItemIconImage)
		return;
	
	if(!IconTexture.IsNull())
	{
		ItemIconImage->SetBrushFromSoftTexture(IconTexture, true);
		ItemIconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ItemIconImage->SetBrushFromSoftTexture(nullptr, true);
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden); 
	}
}

void USRSlotWidget::ApplyButtonStyle(const FButtonStyle& InStyle)
{
	if (Button)
	{
		Button->SetStyle(InStyle);
	}
}

void USRSlotWidget::SetSlotButtonNormalStyle(const FButtonStyle& InStyle)
{
	SlotButtonNormalStyle = InStyle;
}

const FButtonStyle& USRSlotWidget::GetSlotButtonNormalStyle()
{
	return SlotButtonNormalStyle;
}

void USRSlotWidget::SetSlotButtonSelectedStyle(const FButtonStyle& InStyle)
{
	SlotButtonSelectedStyle = InStyle;
}

const FButtonStyle& USRSlotWidget::GetSlotButtonSelectedStyle()
{
	return SlotButtonSelectedStyle;
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

	check(SlotDragVisualWidgetClass);

	if (OutOperation == nullptr)
	{	
		USRDragDropOperation* DragDropOper = NewObject<USRDragDropOperation>();
		USRSlotDragVisualWidget* DragPreview = CreateWidget<USRSlotDragVisualWidget>(GetWorld(), SlotDragVisualWidgetClass); // 드래그 미리보기 위젯 클래스

		check(DragDropOper);
		check(DragPreview);

		DragDropOper->OnDragCancelled.AddDynamic(this, &ThisClass::SlotDragCancelled);

		DragPreview->SetDragVisualImage(ItemData.Icon);

		DragDropOper->DefaultDragVisual = DragPreview;
		DragDropOper->DraggedSlot = this;
		DragDropOper->DraggedSlotItemData = ItemData;
		OutOperation = DragDropOper;

		//빈슬롯으로 세팅해줍니다.
		Clear();
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

		OnSlotDropedDelegate.Broadcast(this, DragDropOper->DraggedSlot);


		UE_LOG(LogTemp, Warning, TEXT("Draged : %s, Droped %s"), *DragDropOper->DraggedSlot->GetName(), *this->GetName());
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

	Fill(SlotItemData);
}

