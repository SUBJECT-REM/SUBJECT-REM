// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRSlotDragVisualWidget.h"
#include "Components/Image.h"

void USRSlotDragVisualWidget::SetDragVisualImage(UTexture2D* Icon)
{
	check(Icon);

	DragVisualImage->SetBrushFromTexture(Icon);
}
