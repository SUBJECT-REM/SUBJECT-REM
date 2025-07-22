// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRInvestigationMenu.h"
#include "UI/SRInventoryWidget.h"
#include "UI/SRClueWidget.h" 
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Presenter/SRClueWidgetPresenter.h"
#include "Presenter/SRInventoryPresenter.h"

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
	OpenOnlyWidget(InventoryWidget);
}

void USRInvestigationMenu::OpenClue()
{
	OpenOnlyWidget(ClueWidget);
}

void USRInvestigationMenu::OpenClueMap()
{
	//아직 ClueMap이 없습니다.
	OpenOnlyWidget(nullptr);
}

void USRInvestigationMenu::NativeConstruct()
{
	InventoryButton->OnClicked.AddDynamic(this, &ThisClass::OpenInventory);
	ClueButton->OnClicked.AddDynamic(this, &ThisClass::OpenClue);
	ClueMapButton->OnClicked.AddDynamic(this, &ThisClass::OpenClueMap);
}


