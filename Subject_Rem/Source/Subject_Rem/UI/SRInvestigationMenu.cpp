// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRInvestigationMenu.h"
#include "UI/SRInventoryWidget.h"
#include "UI/ClueMap/SRClueMapWidget.h"
#include "UI/SRClueWidget.h" 
#include "Actor/Tutorial/SRTutorialManager.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Presenter/SRClueWidgetPresenter.h"
#include "Presenter/SRInventoryPresenter.h"
#include "Kismet/GameplayStatics.h"
#include "SRGameplayTags.h"
#include "Animation/WidgetAnimation.h"
#include "Actor/SRCaptionManagerActor.h"




void USRInvestigationMenu::InitInvestigationMenuWidget(UActorComponent* DataSourceComp)
{
	check(DataSourceComp);

	if (!InvenPresenter)
	{
		InvenPresenter = NewObject<USRInventoryPresenter>(this);
	}
	if (!CluePresenter)
	{
		CluePresenter = NewObject<USRClueWidgetPresenter>(this);
	}

	check(InvenPresenter);
	check(CluePresenter);

	InvenPresenter->Init(DataSourceComp, InventoryWidget);
	CluePresenter->Init(DataSourceComp, ClueWidget);

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASRTutorialManager::StaticClass());
	if (FoundActor)
	{
		 TutorialManager = Cast<ASRTutorialManager>(FoundActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial Manager not vaild"));
	}
	if (TutorialManager)
	{
		TutorialManager->OnTutorialStartDelegate.AddDynamic(this, &ThisClass::OnTutorialStart);
		TutorialManager->OnTutorialCompleteDelegate.AddDynamic(this, &ThisClass::OnTutorialComplete);
	}
}

void USRInvestigationMenu::OpenOnlyWidget(UUserWidget* WantOpenWidget)
{	
	for (UWidget* Widget : MenuOverlay->GetAllChildren())
	{
		if (!Widget) continue;
		
		Widget->SetVisibility(Widget == WantOpenWidget ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void USRInvestigationMenu::OpenInventory()
{
	ChangeButtonZOrder(InventoryButton, -1);
	ChangeButtonZOrder(ClueMapButton, 0);
	ChangeButtonZOrder(ClueButton, 0);
	OpenOnlyWidget(InventoryWidget);

	InventoryButton->SetStyle(InventoryButtonSelectedStyle);
	ClueButton->SetStyle(ClueButtonNormalStyle);
	ClueMapButton->SetStyle(ClueMapButtonNormalStyle);

	PlayAnimation(OpenInventoryAnim);
}

void USRInvestigationMenu::OpenClue()
{
	ChangeButtonZOrder(ClueButton, -1);
	ChangeButtonZOrder(InventoryButton, 0);
	ChangeButtonZOrder(ClueMapButton, 0);
	OpenOnlyWidget(ClueWidget);

	NotifyClueButtonClick();

	InventoryButton->SetStyle(InventoryButtonNormalStyle);
	ClueButton->SetStyle(ClueButtonSelectedStyle);
	ClueMapButton->SetStyle(ClueMapButtonNormalStyle);

	PlayAnimation(OpenClueAnim);
	
}

void USRInvestigationMenu::OpenClueMap()
{
	ChangeButtonZOrder(ClueMapButton, -1);
	ChangeButtonZOrder(InventoryButton, 0);
	ChangeButtonZOrder(ClueButton, 0);
	//아직 ClueMap이 없습니다.
	OpenOnlyWidget(ClueMapWidget);

	NotifyClueMapButtonClick();

	InventoryButton->SetStyle(InventoryButtonNormalStyle);
	ClueButton->SetStyle(ClueButtonNormalStyle);
	ClueMapButton->SetStyle(ClueMapButtonSelectedStyle);

	PlayAnimation(OpenClueMapAnim);
}

void USRInvestigationMenu::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(OpenMenu);
	if (CaptionManager.IsValid())
	{
		CaptionManager->NotifyInvestigationToggle(true);
	}
}

void USRInvestigationMenu::HideWidget()
{
	PlayAnimation(CloseMenu);
}

void USRInvestigationMenu::HideWidgetAnimFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (CaptionManager.IsValid())
	{
		CaptionManager->NotifyInvestigationToggle(false);
	}
}

void USRInvestigationMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryButton->OnClicked.AddDynamic(this, &ThisClass::OpenInventory);
	ClueButton->OnClicked.AddDynamic(this, &ThisClass::OpenClue);
	ClueMapButton->OnClicked.AddDynamic(this, &ThisClass::OpenClueMap);

	OnVisibilityChanged.AddDynamic(this, &ThisClass::HandleVisibilityChange);

	ClueClickPulse->SetVisibility(ESlateVisibility::Hidden);
	ClueMapClickPulse->SetVisibility(ESlateVisibility::Hidden);


	CloseAnimFinishedDelegate.BindDynamic(this, &ThisClass::HideWidgetAnimFinished);
	BindToAnimationFinished(CloseMenu, CloseAnimFinishedDelegate);

	//GetCaptionManager();
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASRCaptionManagerActor::StaticClass());
	if (FoundActor)
	{
		CaptionManager = Cast<ASRCaptionManagerActor>(FoundActor);
	}

}

void USRInvestigationMenu::ChangeButtonZOrder(UButton* Widget,int8 NewZOrder)
{
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	check(CanvasPanelSlot);

	CanvasPanelSlot->SetZOrder(NewZOrder);
}

void USRInvestigationMenu::HandleVisibilityChange(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible && TutorialManager)
	{
		TutorialManager->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_OepnInvenstigation);
		OnVisibilityChanged.RemoveDynamic(this, &ThisClass::HandleVisibilityChange);
	}
}

void USRInvestigationMenu::OnTutorialStart(FGameplayTag Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("Menu OnTutorialStart Tag Name : %s"), *Tag.ToString());
	if (Tag == SRGameplayTags::Tutorial_ID_ClickClueButton)
	{
		////ClueButton을 클릭하도록 하고 다른 UI클릭은 못하도록합니다.
		ClueClickPulse->SetVisibility(ESlateVisibility::Visible);
		//InventoryButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		//ClueMapButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (Tag == SRGameplayTags::Tutorial_ID_ClickClueMapButton)
	{
		ClueMapClickPulse->SetVisibility(ESlateVisibility::Visible);
		//InventoryButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		//ClueButton->SetVisibility(ESlateVisibility::HitTestInvisible);*/
	}

}

void USRInvestigationMenu::OnTutorialComplete(FGameplayTag Tag)
{
	if (Tag == SRGameplayTags::Tutorial_ID_ClickClueButton)
	{
		//다른 입력을 다시 활성화하도록함.
		ClueClickPulse->SetVisibility(ESlateVisibility::Hidden);
	/*	InventoryButton->SetVisibility(ESlateVisibility::Visible);
		ClueMapButton->SetVisibility(ESlateVisibility::Visible);*/
	}
	else if (Tag == SRGameplayTags::Tutorial_ID_ClickClueMapButton)
	{
		ClueMapClickPulse->SetVisibility(ESlateVisibility::Hidden);
		//InventoryButton->SetVisibility(ESlateVisibility::Visible);
		//ClueButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USRInvestigationMenu::NotifyClueButtonClick()
{
	if (TutorialManager)
	{
		TutorialManager->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_ClickClueButton);
	}
}

void USRInvestigationMenu::NotifyClueMapButtonClick()
{
	if (TutorialManager)
	{
		TutorialManager->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_ClickClueMapButton);
	}
}


