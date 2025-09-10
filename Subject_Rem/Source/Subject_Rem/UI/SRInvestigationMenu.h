// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
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
class USRClueMapWidget;
class USRInventoryPresenter;
class USRClueWidgetPresenter;
class UHorizontalBox;
class ASRTutorialManager;
class ASRCaptionManagerActor;


UCLASS(Blueprintable)
class SUBJECT_REM_API USRInvestigationMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitInvestigationMenuWidget(UActorComponent* DataSourceComp);

	void OpenOnlyWidget(UUserWidget* WantOpenWidget);

	UFUNCTION()
	void OpenInventory();

	UFUNCTION()
	void OpenClue();

	UFUNCTION()
	void OpenClueMap();

	//Open Widget Animation
	UFUNCTION(BlueprintCallable)
	void ShowWidget();

	//Close Widget Animation
	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION()
	void HideWidgetAnimFinished();



protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle InventoryButtonNormalStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle InventoryButtonSelectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueButtonNormalStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueButtonSelectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueMapButtonNormalStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Styles")
	FButtonStyle ClueMapButtonSelectedStyle;
private:
	void ChangeButtonZOrder(UButton* Widget, int8 NewZOrder);

	UFUNCTION()
	void HandleVisibilityChange(ESlateVisibility InVisibility);

	UFUNCTION()
	void OnTutorialStart(FGameplayTag Tag);

	UFUNCTION()
	void OnTutorialComplete(FGameplayTag Tag);

	void NotifyClueButtonClick();

	void NotifyClueMapButtonClick();

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
	USRClueMapWidget* ClueMapWidget;

	UPROPERTY(meta = (BindWidget))
	UOverlay* MenuOverlay;

	UPROPERTY()
	USRInventoryPresenter* InvenPresenter;

	UPROPERTY()
	USRClueWidgetPresenter* CluePresenter;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ClueClickPulse;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ClueMapClickPulse;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenInventoryAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenClueAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenClueMapAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenMenu;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* CloseMenu;

	FWidgetAnimationDynamicEvent CloseAnimFinishedDelegate;

	//Presenter에서 처리할라했지만 아직 InvestigationMenu에서는 다른 동작이 없기에 새 Presenter를 만들지 않았음.
	//추후 게임로직과 연결되는 부분이 점점 생긴다면 Presenter를 만들것같음.
	UPROPERTY()
	ASRTutorialManager* TutorialManager;

	UPROPERTY()
	TWeakObjectPtr<ASRCaptionManagerActor> CaptionManager;
};
