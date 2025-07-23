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
class UButton;
class USRSlotWidget;
class USRClueCombineResultWidget;

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
	void UpdateClueCombineResultWidget(const FSRItemBaseData& Data);
	
	FCombineButtonClickedSignature CombineButtonClickedDelegate;
protected:
	virtual void NativeConstruct() override;

private:

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

	/*단서들을 보여주는 패널*/
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ClueGridPanel;

	/*단서 조합 패널*/
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ClueCombineGridPanel;

	/*단서 조합 결과*/
	UPROPERTY(meta = (BindWidget))
	USRClueCombineResultWidget* ClueCombineResultWidget;

	/*단서 조합 버튼*/
	UPROPERTY(meta = (BindWidget))
	UButton* ClueCombineButton;
	
	const int VaildCombineItemNum = 2;
};
