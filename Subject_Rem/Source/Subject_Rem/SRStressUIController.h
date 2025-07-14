// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRStressUIController.generated.h"

class UProgressBar;
class UImage;
class USRStressLocalPlayerSubsystem;

UCLASS()
class SUBJECT_REM_API USRStressUIController : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StressProgressBar;
	float StressProgressBarColor_G = 0.05;
	float StressProgressBarColor_B = 0.05;

	UPROPERTY(meta = (BindWidget))
	UImage* HighStressEffect;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Pulse;

private:
	void UpdateStressProgressBar(float StressAmount);
	USRStressLocalPlayerSubsystem* StressSubsystem;

	/*
	* 스트레스 이미지의 알파값(투명도)를 조절합니다.
	* @param 이미지의 투명도 (0.0-1.0)
	*/
	void SetHightStressEffectAlpha(float Alpha);

	/*
	* StressEffect를 재생하는 함수입니다.
	* @param true = 재생, flase = 멈춤
	*/
	void PlayDangerPulse(bool bIsPlay);


};
