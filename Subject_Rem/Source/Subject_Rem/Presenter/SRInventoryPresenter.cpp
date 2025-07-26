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
	
	InvenComp->AddInventoryDataDelegate.AddDynamic(this, &ThisClass::RequestAddInventoryWidget);
	InvenComp->RemoveInventoryDataDelegate.AddDynamic(this, &ThisClass::RequsetRemoveInventoryWidget);

	InventoryWidget = Cast<USRInventoryWidget>(InitWidget);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget Cast cannot be cast to ClueWidget"));
		return;
	}
}

void USRInventoryPresenter::RequestAddInventoryWidget(const FSRItemBaseData& Data)
{
	InventoryWidget->AddItemInventoryGridPanel(Data);
}

void USRInventoryPresenter::RequsetRemoveInventoryWidget(const TArray<FName>& ItemIds)
{
	InventoryWidget->RemoveItemInventoryGridPanel(ItemIds);
	InventoryWidget->UpdateItemName(NAME_None);
	InventoryWidget->UpdateItemDes(NAME_None);
}
