// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRClueData.h"
#include "SRClueWidget.generated.h"

/**
 * 
 */
class UGridPanel;
class UButton;
class USRSlotWidget;
UCLASS()
class SUBJECT_REM_API USRClueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*
	* ClueGridPanel을 갱신하는 함수입니다.
	* @param 갱신하기 위한 데이터
	*/	void UpdateClueWidget(const FSRClueData& Data);

protected:
	virtual void NativeConstruct();

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

	/*단서들을 보여주는 패널*/
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ClueGridPanel;

	/*단서 조합 패널*/
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ClueCombineGridPanel;

};
