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

private:
	void UpdateStressProgressBar(float StressAmount);
	USRStressLocalPlayerSubsystem* StressSubsystem;


};
