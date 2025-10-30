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

	// 슬롯(순서 기반)
	UPROPERTY()
	TArray<TObjectPtr<USRClueMapCombinedResultWidget>> ClueMapResults;

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

	void HandleTrueClueLinkWidget(FName CombinedClueName);
	UFUNCTION()
	void HandleCombinedClue(const FSRClueMapUIData& Data);

	void FindPlayerInventoryComponent();


	//진실 단서만 담습니다.
	TArray<FName> TrueClues;


	TArray<USRTrueClueLinkWidget*> TrueClueLinkWidgets;

	void UpdateClueMapProgressBar(float NewPercent);
	void UpdatePercentTextBlock(float NewPercent);
private:
	USRInventoryComponent* InventoryComponent;

	//TODO: DatTable에서 TrueClueMap 개수 개져와서 설정하기.

	UFUNCTION()
	void UpdateClueMapCombinedResultDescriptionWidget(const FName& Id);

	void InitializeSlots();         // 컨테이너 자식 스캔해서 Slots 배열 구성
	void InitializeLinks();         // 링크 자식 스캔해서 LinkWidgets 구성
	void ResetClueMapUI();          // 슬롯/링크/진행도 초기화

	UPROPERTY(EditDefaultsOnly)
	int32  MaxTrueClueMapNum = 12;

	float GetProgressRatio() const; // 추가

	// 캐시 추가
	TMap<FName, FSRClueMapUIData> CachedUIByClueId;

	int32 MaxClueMapNum =0;
	int32 NextFillIndex = 0;

	UPROPERTY(EditDefaultsOnly)
	FVector2D TwoClueBasedClueMapSize;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ThreeClueBasedClueMapSize;

};
