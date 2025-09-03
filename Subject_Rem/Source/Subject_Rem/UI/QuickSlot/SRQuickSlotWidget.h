// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRQuickSlotWidget.generated.h"

/**
 * 
 */
class UGridPanel;
class UDataTable;
UCLASS()
class SUBJECT_REM_API USRQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	
private:
	
	UFUNCTION()
	void OnSlotChange(int32 Index, TSoftObjectPtr<UTexture2D> Icon);

	UPROPERTY(meta = (BindWidget))
	UGridPanel* QuickSlotGridPanel;

};
