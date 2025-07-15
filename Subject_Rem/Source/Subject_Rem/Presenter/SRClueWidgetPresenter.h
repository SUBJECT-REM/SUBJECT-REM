// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Presenter/SRPresenter.h"
#include "SRClueData.h"
#include "SRClueWidgetPresenter.generated.h"

/**
 * 
 */
class USRInventoryComponent;
class USRClueWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateClueWidgetSignature, const FSRClueData&, Data);

UCLASS(BlueprintType, Blueprintable)
class SUBJECT_REM_API USRClueWidgetPresenter : public USRPresenter
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Init(UActorComponent* InitComponent, UUserWidget* InitWidget) override;

private:
	/*
	* 인벤토리 컴포넌트의 ClueDatas 변경사항에 대한 델리게이트 바인딩 함수입니다. ClueWidget을 갱신합니다.
	* @param 추가된 데이터
	*/
	UFUNCTION()
	void RequestUpdateClueWidget(const FSRClueData& Data);

	UFUNCTION()
	void RequestCombineClue(TArray<FName> ClueIds);

	UPROPERTY()
	USRInventoryComponent* InvenComp;

	UPROPERTY()
	USRClueWidget* ClueWidget;

};
