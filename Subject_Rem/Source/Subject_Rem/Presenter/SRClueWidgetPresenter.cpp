// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRClueWidgetPresenter.h"
#include "SRClueWidgetPresenter.h"
#include "UI/SRClueWidget.h"
#include "Component/SRInventoryComponent.h"

void USRClueWidgetPresenter::Init(UActorComponent* InitComponent, UUserWidget* InitWidget)
{
	InvenComp = Cast<USRInventoryComponent>(InitComponent);
	if (!InvenComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitComponent Cast cannot be cast to InventoryComponent"));
		return;
	}
	check(InvenComp);
	InvenComp->ChangeClueDatasDelegate.AddDynamic(this, &ThisClass::UpdateClueWidget);

	ClueWidget = Cast<USRClueWidget>(InitWidget);
	if (!ClueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget Cast cannot be cast to ClueWidget"));
		return;
	}
	check(ClueWidget)

}

void USRClueWidgetPresenter::UpdateClueWidget(const FSRClueData& Data)
{
	check(ClueWidget)

	ClueWidget->UpdateClueWidget(Data);
}
