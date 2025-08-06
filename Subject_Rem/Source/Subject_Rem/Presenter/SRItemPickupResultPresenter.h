// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Presenter/SRPresenter.h"
#include "SRItemPickupResultPresenter.generated.h"

/**
 * 
 */
class USRInventoryComponent;
class USRItemPickupResultWidget;
UCLASS(Blueprinttype, Blueprintable)
class SUBJECT_REM_API USRItemPickupResultPresenter : public USRPresenter
{
	GENERATED_BODY()
	
public:
	virtual void Init(UActorComponent* InitComponent, UUserWidget* InitWidget) override;

	UFUNCTION()
	void ShowItemPickWidget(const FSRItemBaseData& ShownItemData);

	UFUNCTION()
	void HandleWidgetVisibilityChanged(ESlateVisibility NewVisibility);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ItemPickUpResultWidgetClass;

	UPROPERTY()
	USRInventoryComponent* InvenComp;

	USRItemPickupResultWidget* ItemPickupResultWidget;
};
