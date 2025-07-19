// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SRRotateItemPreviewWidget.h"
#include "Actor/SmoothRotateActor/SRSmoothRotateActor.h"

void USRRotateItemPreviewWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Log, TEXT("NativeConstruct"));
}

void USRRotateItemPreviewWidget::NativeDestruct()
{
	UE_LOG(LogTemp, Log, TEXT("NativeDestruct"));
	bDragging = false;
}

FReply USRRotateItemPreviewWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtonDown"));
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		bDragging = true;
		LastMousePosition = InMouseEvent.GetScreenSpacePosition();

		// 마우스 캡처 생략해도 무방
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply USRRotateItemPreviewWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bDragging)
	{
		UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtonMove"));
	}
	if (bDragging && TargetActor)
	{
		FVector2D CurrentPosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D Delta = CurrentPosition - LastMousePosition;

		TargetActor->AddRotationInput(Delta * 0.2f); // 조정 가능

		LastMousePosition = CurrentPosition;
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply USRRotateItemPreviewWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtoUp"));
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

void USRRotateItemPreviewWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UE_LOG(LogTemp, Log, TEXT("NativeOnMouseLeave"));
	bDragging = false;
}
