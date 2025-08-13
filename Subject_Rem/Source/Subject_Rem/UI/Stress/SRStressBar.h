// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRStressBar.generated.h"

class USRStressLocalPlayerSubsystem;
class UHorizontalBox;
class UProgressBar;

UCLASS()
class SUBJECT_REM_API USRStressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox;
	UPROPERTY()
	TArray<UProgressBar*> ProgressBarArray;
private:
	USRStressLocalPlayerSubsystem* StressSubsystem;
	void UpdateStressProgressBar(float StressAmount);
	void CollectProgressBars();
};
