// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRItemData.h"
#include "SRClueWidget.generated.h"

/**
 * 
 */
class UGridPanel;
class UUniformGridPanel;
class UButton;
class USRSlotWidget;
class USRClueCombineResultWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveDeviceDataSignature, FName, Id);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombineButtonClickedSignature, TArray<FName>, ClickedSlot);

UCLASS()
class SUBJECT_REM_API USRClueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*
	* ClueGridPanel을 갱신하는 함수입니다.
	* @param 갱신하기 위한 데이터
	*/
	void UpdateClueGridWidget(const FSRItemBaseData& Data);
	void UpdateClueCombineResultWidget(const FSRClueMapUIData& Data);
	void UpdateDeviceGridWidget(const FSRDeviceUIData& Data);

	FRemoveDeviceDataSignature RemoveDeviceDataDelegate;
	FCombineButtonClickedSignature CombineButtonClickedDelegate;
protected:
	virtual void NativeConstruct() override;

	/*단서들을 보여주는 패널*/
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* ClueGridPanel;

	/*디바이스들을 보여주는 패널*/
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* DeviceGridPanel;
	
	/*단서 조합 패널*/
	//디바이스에 따른 위젯스위처
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ClueCombineSwitcher;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* ClueCombineGridPanel;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* One_ClueCombine;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGridPanel* Three_ClueCombineGridPanel;
private:
	//디바이스에 해당하는 데이터를 넣기 위한 헬퍼함수들
	bool CanAccepDropClueGridPanel(const FSRItemBaseData& Item, USRSlotWidget* From, USRSlotWidget* To);
	bool ValidateClueCombineDrop(const FSRItemBaseData& Item, USRSlotWidget* From, USRSlotWidget* To);
	void RevertDrop(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot, const FSRItemBaseData& MovedData);

	//디바이스 비활성화, 다른 디바이스 활성화시 에 대한처리를 위한 헬퍼함수
	void DeactivateCurrentDevice();                        
	void MoveAllFromCombineToClue(UGridPanel* FromGrid);   
	USRSlotWidget* FindFirstEmptyClueSlot() const;         

	UFUNCTION()
	void OnSlotDropped_ClueCombine(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot);
	/*
	* ClueGridPanel 클릭시 ClueCombineGridPanel로 데이터 이동 및 위젯을 갱신합니다.
	* @param 클릭한 슬롯 위젯
	*/
	UFUNCTION()
	void ClueDataMoveToClueCombine(USRSlotWidget* ClickedSlot);

	/*
	* ClueCombineGridPanel클릭시 ClueGridPanel 로 데이터 이동 및 위젯을 갱신합니다.
	* @param 클릭한 슬롯 위젯
	*/
	UFUNCTION()
	void ClueCombineDataMoveToClue(USRSlotWidget* ClickedSlot);

	/*
	* CombineButton 클릭시 아이템들이 조합되도록 델리게이트를 Broadcast합니다.
	*/
	UFUNCTION()
	void OnClickedCombineButton();

	UFUNCTION()
	void OnClickedDeviceSlot(USRSlotWidget* ClickedSlot);

	UGridPanel* GetCurrentClueCombineGrid();
	/*단서 조합 결과*/
	UPROPERTY(meta = (BindWidget))
	USRClueCombineResultWidget* ClueCombineResultWidget;

	/*단서 조합 버튼*/
	UPROPERTY(meta = (BindWidget))
	UButton* ClueCombineButton;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueSlotButtonNormalStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueSlotButtonSelectedStyle;

	int CurVaildCombineItemNum = 2;

	TMap<FName, uint8> CashedDeviceUsingClueNum;

	UPROPERTY()
	USRSlotWidget* CurUsingDevicedSlot;

	TMap<FName, TArray<FName>> CachedDeviceAllowedMap;

	const int DefaultVaildCombineItemNum = 2;
	const int DefaultClueCombinePanelIndex = 1;
};
