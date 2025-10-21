// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRClueCombineWidget.generated.h"

class UPanelWidget;
class UImage;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombineSlotDropped, USRSlotWidget*, Dropped, USRSlotWidget*, Dragged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombineButtonClicked);

UCLASS()
class SUBJECT_REM_API USRClueCombineWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable) 
	void SetLayoutByCount(int32 Count); // 1,2,3

	UFUNCTION(BlueprintCallable)
	UPanelWidget* GetActivePanelWidget();

	void PutBackDevice();
	void HandleOptionalResource(USRSlotWidget* DroppedSlot, TSoftObjectPtr<UTexture2D> Resource);

	UPROPERTY(BlueprintAssignable) 
	FOnCombineSlotDropped OnCombineSlotDropped;
	UPROPERTY(BlueprintAssignable) 
	FOnCombineButtonClicked OnCombineButtonClicked;

	UPROPERTY(EditAnywhere)
	UTexture2D* NormalClueBar;
protected:
	/*단서 조합 패널*/
	//디바이스에 따른 위젯스위처
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ClueCombineSwitcher;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* OnePanel;

	UPROPERTY(meta = (BindWidget))
	UImage* ClueBar;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* TwoPanel;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ThreePanel;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ThreeOptionalPanel;

	UPROPERTY()
	UPanelWidget* CurrentActivePanel;

	UFUNCTION()
	void OnSlotDropped(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot);

};
