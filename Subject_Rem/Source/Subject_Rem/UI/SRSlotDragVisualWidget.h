// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRSlotDragVisualWidget.generated.h"

/**
 * 
 */
class UImage;
class UTexture2D;
UCLASS(Blueprintable)
class SUBJECT_REM_API USRSlotDragVisualWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetDragVisualImage(UTexture2D* Icon);
private:

	UPROPERTY(meta=(BindWidget))
	UImage* DragVisualImage;
};
