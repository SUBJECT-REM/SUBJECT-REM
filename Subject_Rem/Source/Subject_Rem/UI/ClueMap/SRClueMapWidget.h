// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemData.h"
#include "SRClueMapWidget.generated.h"

class UCanvasPanel;
class USRInventoryComponent;
class USRClueMapCombinedResultWidget;
class USRTrueClueLinkWidget;
class UProgressBar;
class URichTextBlock;
class USRCluemapCombinedDesWidget;

UCLASS()
class SUBJECT_REM_API USRClueMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvasPanel;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ClueMapProgressBar;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* PercentTextBlock;

	UPROPERTY(meta = (BindWidget))
	USRCluemapCombinedDesWidget* ClueMapCombinedDesWidget;
protected:
	bool bIsInitialized = false;

	//InventoryComponent와 Bind합니다.
	void BindInventoryDelegate();
	//현재 사용하지 않음.
	void UnbindInventoryDelegate();

	//CombineClue
	//최초에 한번만 실행하며 CombinedClueWidget, LinkWidget
	void FindCombinedResultWidgets();
	void FindTrueClueLinkWidgets();

	USRClueMapCombinedResultWidget* FindCombinedClueResultWidget(FName CombinedClueName);
	void HandleTrueClueLinkWidget(FName CombinedClueName);
	UFUNCTION()
	void HandleCombinedClue(const FSRClueMapUIData& Data);

	void FindPlayerInventoryComponent();

	//Widget들만 모아서 미리 찾아 Array에 넣는 Array
	TMap<FName, USRClueMapCombinedResultWidget*> CombinedClueWidgets;
	//진실 단서만 담습니다.
	TArray<FName> TrueClues;


	TMap<FName,USRTrueClueLinkWidget*> TrueClueLinkWidgets;

	void UpdateClueMapProgressBar(float NewPercent);
	void UpdatePercentTextBlock(float NewPercent);
private:
	USRInventoryComponent* InventoryComponent;

	//TODO: DatTable에서 TrueClueMap 개수 개져와서 설정하기.

	UFUNCTION()
	void UpdateClueMapCombinedResultDescriptionWidget(const FName& Id);

	UPROPERTY(EditDefaultsOnly)
	int32  MaxTrueClueMapNum = 12;

	float GetProgressRatio() const; // 추가

	// 캐시 추가
	TMap<FName, FSRClueMapUIData> CachedUIByClueId;

};
