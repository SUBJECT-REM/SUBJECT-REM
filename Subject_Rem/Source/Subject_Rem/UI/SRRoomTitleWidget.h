// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TimerHandle.h"
#include "SRRoomTitleWidget.generated.h"

/**
 * 
 */
class UImage;

UCLASS()
class SUBJECT_REM_API USRRoomTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetRoomTitleImage(TSoftObjectPtr<UTexture2D> NewImage);

	void SetRoomTitleImageSize(FVector2D NewSize);

	void ShowRoomTitleWidget();
private:
	void PlayHideWidgetAnim();

	void HideWidgetAnimFinished();

	UPROPERTY(meta = (BindWidget))
	UImage* RoomTitleImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenAnim;

	FTimerHandle  WidgetHidenTimerHandler;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* CloseAnim;

	UPROPERTY(EditDefaultsOnly)
	float PendingVisibleSeconds=3.f;

	FWidgetAnimationDynamicEvent CloseAnimFinishedDelegate;
};
