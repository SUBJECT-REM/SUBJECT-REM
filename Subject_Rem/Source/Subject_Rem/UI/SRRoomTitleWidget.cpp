// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRRoomTitleWidget.h"
#include "Components/Image.h"

void USRRoomTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USRRoomTitleWidget::SetRoomTitleImage(TSoftObjectPtr<UTexture2D> NewImage)
{
	if (!NewImage.IsNull())
	{
		RoomTitleImage->SetBrushFromSoftTexture(NewImage);
	}
}

void USRRoomTitleWidget::SetRoomTitleImageSize(FVector2D NewSize)
{
	RoomTitleImage->SetDesiredSizeOverride(NewSize);
	//RoomTitleImage->SetRenderTransform()
}

void USRRoomTitleWidget::ShowRoomTitleWidget()
{
	
	SetVisibility(ESlateVisibility::Visible);
	// 3초 후 닫기 시작
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(WidgetHidenTimerHandler,this,&ThisClass::CollapseWidget, PendingVisibleSeconds ,false);
	}
}

void USRRoomTitleWidget::CollapseWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
