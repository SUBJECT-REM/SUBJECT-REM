// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRClueData.h"
#include "SRSlotWidget.generated.h"

/**
 * 
 */

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClickedSignature, USRSlotWidget*, ClickedSlot);

UCLASS()
class SUBJECT_REM_API USRSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnSlotClickedSignature FOnSlotClickedDelegate;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	/*
	* 버튼 이미지를 지정합니다.
	* @param 버튼 이미지 텍스처
	*/
	void SetSlotStyle(UObject* Icon);

	void SetClueData(const FSRClueData& NewData);
	const FSRClueData& GetClueData();
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnButtonClicked();

	FSRClueData ClueData;
};
