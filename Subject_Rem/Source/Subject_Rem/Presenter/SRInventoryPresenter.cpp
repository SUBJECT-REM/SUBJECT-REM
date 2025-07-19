// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRInventoryPresenter.h"
#include "Component/SRInventoryComponent.h"
#include "UI/SRInventoryWidget.h"

void USRInventoryPresenter::Init(UActorComponent* InitComponent, UUserWidget* InitWidget)
{
	Super::Init(InitComponent, InitWidget);

	InvenComp = Cast<USRInventoryComponent>(InitComponent);
	if (!InvenComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitComponent Cast cannot be cast to InventoryComponent"));
		return;
	}
	check(InvenComp);
	
	if (!InvenComp->ChangeInventoryDataDelegate.IsAlreadyBound(this, &ThisClass::RequestUpdateInventoryWidget))
	{
		InvenComp->ChangeInventoryDataDelegate.AddDynamic(this, &ThisClass::RequestUpdateInventoryWidget);
	}

	InventoryWidget = Cast<USRInventoryWidget>(InitWidget);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget Cast cannot be cast to ClueWidget"));
		return;
	}
}

void USRInventoryPresenter::RequestUpdateInventoryWidget(const FSRItemBaseData& Data)
{
	InventoryWidget->UpdateInventoryGridPanel(Data);
}
