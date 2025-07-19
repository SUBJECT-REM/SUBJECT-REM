// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SRRotateItemPreviewWidget.h"
#include "Actor/SmoothRotateActor/SRSmoothRotateActor.h"
#include "Kismet/GameplayStatics.h"

void USRRotateItemPreviewWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Log, TEXT("NativeConstruct"));


	// 액터 찾기
	if (TargetActor) return;

	//0.5초 뒤에 찾기
	FTimerHandle FindTargetActorTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FindTargetActorTimerHandle, this, &ThisClass::GetTargetActorFromLevel, 0.5f, false);

	//바로 찾기.
	//GetTargetActorFromLevel()
}

void USRRotateItemPreviewWidget::NativeDestruct()
{
	//UE_LOG(LogTemp, Log, TEXT("NativeDestruct"));
	bDragging = false;
}

FReply USRRotateItemPreviewWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtonDown"));
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
		//UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtonMove"));
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

void USRRotateItemPreviewWidget::GetTargetActorFromLevel()
{
	// 액터 찾기
	if (TargetActor) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASRSmoothRotateActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		TargetActor = Cast<ASRSmoothRotateActor>(FoundActors[0]);
		UE_LOG(LogTemp, Warning, TEXT("TargetActor Found: %s"), *TargetActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASRSmoothRotateActor not found"));
	}
}
