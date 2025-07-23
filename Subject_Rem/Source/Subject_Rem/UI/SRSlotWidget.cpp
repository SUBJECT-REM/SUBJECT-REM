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

void USRSlotWidget::SetIsOccupied(bool IsOccupied)
{
	bIsOccupied = IsOccupied;
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
		InMouseEvent.GetScreenSpacePosition();
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}


	return Reply.NativeReply;
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

			
			DragPreview->SetSlotStyle(ItemData.Icon);

			OutOperation = DragDropOper;
			DragDropOper->DefaultDragVisual = DragPreview;
			DragDropOper->DraggedSlot = this;

			SetSlotStyle(nullptr);

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

	}

	return true;
}	
