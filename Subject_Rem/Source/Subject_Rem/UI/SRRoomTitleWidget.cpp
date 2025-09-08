// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRRoomTitleWidget.h"
#include "Components/Image.h"

void USRRoomTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseAnimFinishedDelegate.BindDynamic(this, &ThisClass::HideWidgetAnimFinished);
	BindToAnimationFinished(CloseAnim, CloseAnimFinishedDelegate);
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

	PlayAnimation(OpenAnim);

	//default 3초 후 닫기 시작
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(WidgetHidenTimerHandler,this,&ThisClass::PlayHideWidgetAnim, PendingVisibleSeconds ,false);
	}
}

void USRRoomTitleWidget::PlayHideWidgetAnim()
{
	PlayAnimation(CloseAnim);
}

void USRRoomTitleWidget::HideWidgetAnimFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
