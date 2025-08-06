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




void USRInvestigationMenu::InitInvestigationMenuWidget(UObject* DataSource)
{
	check(DataSource);

	UActorComponent* Comp =Cast<UActorComponent>(DataSource);

	check(Comp);
	if (!InvenPresenter)
	{
		InvenPresenter = NewObject<USRInventoryPresenter>(GetWorld());
	}
	if (!CluePresenter)
	{
		CluePresenter = NewObject<USRClueWidgetPresenter>(GetWorld());
	}

	check(InvenPresenter);
	check(CluePresenter);

	InvenPresenter->Init(Comp, InventoryWidget);
	CluePresenter->Init(Comp, ClueWidget);

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASRTutorialManager::StaticClass());
	if (FoundActor)
	{
		 TutorialManager = Cast<ASRTutorialManager>(FoundActor);
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
	ChangeButtonZOrder(InventoryButton, 0);
	ChangeButtonZOrder(ClueMapButton, 2);
	ChangeButtonZOrder(ClueButton, 2);
	OpenOnlyWidget(InventoryWidget);
}

void USRInvestigationMenu::OpenClue()
{
	ChangeButtonZOrder(ClueButton, 0);
	ChangeButtonZOrder(InventoryButton, 2);
	ChangeButtonZOrder(ClueMapButton, 2);
	OpenOnlyWidget(ClueWidget);

	NotifyClueButtonClick();
}

void USRInvestigationMenu::OpenClueMap()
{
	ChangeButtonZOrder(ClueMapButton, 0);
	ChangeButtonZOrder(InventoryButton, 2);
	ChangeButtonZOrder(ClueButton, 2);
	//아직 ClueMap이 없습니다.
	OpenOnlyWidget(ClueMapWidget);
}

void USRInvestigationMenu::NativeConstruct()
{
	InventoryButton->OnClicked.AddDynamic(this, &ThisClass::OpenInventory);
	ClueButton->OnClicked.AddDynamic(this, &ThisClass::OpenClue);
	ClueMapButton->OnClicked.AddDynamic(this, &ThisClass::OpenClueMap);
}

void USRInvestigationMenu::ChangeButtonZOrder(UButton* Widget,int8 NewZOrder)
{
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	check(CanvasPanelSlot);

	CanvasPanelSlot->SetZOrder(NewZOrder);
}

void USRInvestigationMenu::NotifyClueButtonClick()
{
	if (TutorialManager)
	{
		TutorialManager->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_ClickClueButton);
	}
	ClueButtonClickInductionBox->SetVisibility(ESlateVisibility::Hidden);
}


