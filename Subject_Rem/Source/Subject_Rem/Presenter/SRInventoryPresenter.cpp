// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRInventoryPresenter.h"
#include "Component/SRInventoryComponent.h"
#include "Component/Character/SRQuickSlotComponent.h"
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

	//Find QuickSlotComp 
	if (AActor* Owner = InvenComp->GetOwner())
	{
		QuickSlotComp = Owner->FindComponentByClass<USRQuickSlotComponent>();
		if (QuickSlotComp)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Bind QuickComp In InvenPresenter"));
			QuickSlotComp->OnQuickSlotChangedDelegate.AddDynamic(this, &ThisClass::RequestUpdateQuickslotInInventory);
			
			TArray<TSoftObjectPtr<UTexture2D>> Icons;
			QuickSlotComp->GetQuickslotSnapshot(Icons);
			for (int32 i = 0; i < Icons.Num(); ++i)
			{
				InventoryWidget->UpdateQuickSlotGridPanel(i, Icons[i]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("USRQuickSlotComponent not found on %s"), *Owner->GetName());
		}
	}
}

void USRInventoryPresenter::RequestAddInventoryWidget(const FSRItemBaseData& Data)
{
	InventoryWidget->AddItemInventoryGridPanel(Data);
}

void USRInventoryPresenter::RequsetRemoveInventoryWidget(const TArray<FName>& ItemIds)
{
	InventoryWidget->RemoveItemInventoryGridPanel(ItemIds);
	InventoryWidget->UpdateItemName(FText());
	InventoryWidget->UpdateItemDes(FText());
}

void USRInventoryPresenter::RequestUpdateQuickslotInInventory(int32 SlotIndex, TSoftObjectPtr<UTexture2D> Icon)
{
	UE_LOG(LogTemp, Warning, TEXT("Update Quick In Inven"));
	InventoryWidget->UpdateQuickSlotGridPanel(SlotIndex, Icon);
}
