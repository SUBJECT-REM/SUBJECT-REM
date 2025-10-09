// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRStressBar.generated.h"

class USRStressLocalPlayerSubsystem;
class UHorizontalBox;
class UProgressBar;
class USoundBase;
UCLASS()
class SUBJECT_REM_API USRStressBar : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StackedStressBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StressPreviewBar;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* StressIncreseSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* StressDecreaseSound;
private:
	USRStressLocalPlayerSubsystem* StressSubsystem;
	void UpdateStressProgressBar(float StressAmount);

//ColorChange
	void OnStressWillChangeSoon(float ChangeValue, float DelayTime);
	//Tick에서 발동됩니다.
	UFUNCTION(BlueprintCallable)
	void StressChangePreviewColorChange(float InDeltaTime);

	void StopStressChange();

	bool bIsPendingLerp = false;
	float PendingLerpTime = 0.f;
	float PendingLerpDuration = 0.f;
};
