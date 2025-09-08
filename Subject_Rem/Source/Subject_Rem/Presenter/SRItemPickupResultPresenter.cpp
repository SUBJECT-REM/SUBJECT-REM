// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRItemPickupResultPresenter.h"
#include "Component/SRInventoryComponent.h"
#include "UI/SRItemPickupResultWidget.h"
#include "SRItemData.h"
#include "Blueprint/UserWidget.h"


void USRItemPickupResultPresenter::Init(UActorComponent* InitComponent, UUserWidget* InitWidget)
{
	Super::Init(InitComponent, InitWidget);

	InvenComp = Cast<USRInventoryComponent>(InitComponent);
	if (!InvenComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitComponent Cast cannot be cast to InventoryComponent"));
		return;
	}
	check(InvenComp);

	InvenComp->AddInventoryDataDelegate.AddDynamic(this, &ThisClass::ShowItemPickWidget);
}

void USRItemPickupResultPresenter::ShowItemPickWidget(const FSRItemBaseData& ShownItemData)
{
	if (!ItemPickupResultWidget)
	{
		//ItemPickupResultWidget에서는 Space입력시 닫아야함. 여기서 필요시 PlayerController를 넘겨줘야할거같음.
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ItemPickupResultWidget = CreateWidget<USRItemPickupResultWidget>(PC, ItemPickUpResultWidgetClass);
		}
		check(ItemPickupResultWidget);
	}

	ItemPickupResultWidget->SetItemPreview(ShownItemData.Mesh);
	ItemPickupResultWidget->SetItemName(ShownItemData.Name);
	ItemPickupResultWidget->SetItemDes(ShownItemData.Description);

	if (!ItemPickupResultWidget->IsInViewport())
	{
		ItemPickupResultWidget->AddToViewport(10);
	}

	if (ItemPickupResultWidget->GetVisibility() == ESlateVisibility::Hidden || ItemPickupResultWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		ItemPickupResultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USRItemPickupResultPresenter::HandleWidgetVisibilityChanged(ESlateVisibility NewVisibility)
{
	if (NewVisibility == ESlateVisibility::Visible)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
}


