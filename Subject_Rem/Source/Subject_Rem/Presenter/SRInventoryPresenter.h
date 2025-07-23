// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Presenter/SRPresenter.h"
#include "SRItemData.h"
#include "SRInventoryPresenter.generated.h"

/**
 * 
 */
class USRInventoryComponent;
class USRInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryWidgetSignature, const FSRItemBaseData&, Data);


UCLASS()
class SUBJECT_REM_API USRInventoryPresenter : public USRPresenter
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void Init(UActorComponent* InitComponent, UUserWidget* InitWidget) override;

private:
	UFUNCTION()
	void RequestAddInventoryWidget(const FSRItemBaseData& Data);

	UFUNCTION()
	void RequsetRemoveInventoryWidget(const TArray<FName>& ItemIds);

	UPROPERTY()
	USRInventoryComponent* InvenComp;

	UPROPERTY()
	USRInventoryWidget* InventoryWidget;
};
