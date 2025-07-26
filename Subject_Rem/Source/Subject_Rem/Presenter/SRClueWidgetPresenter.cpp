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
	InvenComp->AddClueDatasDelegate.AddDynamic(this, &ThisClass::RequestUpdateClueGridWidget);
	InvenComp->ClueMapResultDelegate.AddDynamic(this, &ThisClass::RequsetUpdateClueCombineResultWidget);

	ClueWidget = Cast<USRClueWidget>(InitWidget);
	if (!ClueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget Cast cannot be cast to ClueWidget"));
		return;
	}
	check(ClueWidget)
	ClueWidget->CombineButtonClickedDelegate.AddDynamic(this, &ThisClass::RequestCombineClue);
}

void USRClueWidgetPresenter::RequestUpdateClueGridWidget(const FSRItemBaseData& Data)
{
	check(ClueWidget)

	ClueWidget->UpdateClueGridWidget(Data);
}

void USRClueWidgetPresenter::RequestCombineClue(TArray<FName> ClueIds)
{
	InvenComp->CombineClue(ClueIds);
}

void USRClueWidgetPresenter::RequsetUpdateClueCombineResultWidget(const FSRItemBaseData& Data)
{
	ClueWidget->UpdateClueCombineResultWidget(Data);
}
