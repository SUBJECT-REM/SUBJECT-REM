// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRSlotDragVisualWidget.h"
#include "Components/Image.h"

void USRSlotDragVisualWidget::SetDragVisualImage(TSoftObjectPtr<UTexture2D> Icon)
{
	check(Icon);
	DragVisualImage->SetBrushFromSoftTexture(Icon);
}
