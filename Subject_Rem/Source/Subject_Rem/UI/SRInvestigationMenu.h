// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRInvestigationMenu.generated.h"

/**
 * Inventory, Clue, ClueMap의 상위 위젯
 * 게임로직 계층을 알려주고 싶진 않았는데 안그러면 Presenter를 어떻게 각각의(Inven,Clue,ClueMap)에 바인딩할지 ..
 * 따라서 최소한의 결합도를 유지해볼라고 UInventoryComp가 아닌 UActorComponent라고 하긴했음.
 * Init함수
 */

class UButton;
class UOverlay;
class USRInventoryWidget;
class USRClueWidget;
class USRInventoryPresenter;
class USRClueWidgetPresenter;

UCLASS(Blueprintable)
class SUBJECT_REM_API USRInvestigationMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitInvestigationMenuWidget(UObject* DataSource);

	void OpenOnlyWidget(UUserWidget* WantOpenWidget);

	UFUNCTION()
	void OpenInventory();

	UFUNCTION()
	void OpenClue();

	UFUNCTION()
	void OpenClueMap();
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ClueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ClueMapButton;

	UPROPERTY(meta = (BindWidget))
	USRInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	USRClueWidget* ClueWidget;

	UPROPERTY(meta = (BindWidget))
	UOverlay* MenuOverlay;

	UPROPERTY()
	USRInventoryPresenter* InvenPresenter;

	UPROPERTY()
	USRClueWidgetPresenter* CluePresenter;
};
