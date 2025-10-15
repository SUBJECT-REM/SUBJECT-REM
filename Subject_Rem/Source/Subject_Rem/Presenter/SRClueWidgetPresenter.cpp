// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRClueWidgetPresenter.h"
#include "SRClueWidgetPresenter.h"
#include "UI/SRClueWidget.h"
#include "Component/SRInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Manager/SRGameFlowManager.h"
#include "Actor/Manager/SRCaptionManagerActor.h"
#include "SRGameplayTags.h"

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
	InvenComp->ClueCombineResultDelegate.AddDynamic(this, &ThisClass::RequsetUpdateClueCombineResultWidget);
	InvenComp->AddDeviceDataDelegate.AddDynamic(this, &ThisClass::RequestUpdateDeviceGridWidget);
	ClueWidget = Cast<USRClueWidget>(InitWidget);
	if (!ClueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget Cast cannot be cast to ClueWidget"));
		return;
	}
	check(ClueWidget)
	ClueWidget->CombineButtonClickedDelegate.AddDynamic(this, &ThisClass::RequestCombineClue);
	ClueWidget->CombineButtonClickedDelegate.AddDynamic(this, &ThisClass::HandleCombineClue);
	ClueWidget->RemoveDeviceDataDelegate.AddDynamic(this, &ThisClass::RequestRemoveDeviceData);
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASRGameFlowManager::StaticClass());
	if (FoundActor)
	{
		GameFlowMng = Cast<ASRGameFlowManager>(FoundActor);
	}
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

void USRClueWidgetPresenter::RequsetUpdateClueCombineResultWidget(const FSRClueCombineResultUIData& Data)
{
	ClueWidget->UpdateClueCombineResultWidget(Data);
}

void USRClueWidgetPresenter::RequestUpdateDeviceGridWidget(const FSRDeviceUIData& Data)
{
	ClueWidget->UpdateDeviceGridWidget(Data);
}

void USRClueWidgetPresenter::HandleCombineClue(TArray<FName> ClueIds)
{
	if (GameFlowMng)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClueCombine Notify Ojbect Completed"));
		GameFlowMng->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_CombineClue);

		if (ClueIds.Num() == 3)
		{
			GameFlowMng->NotifyObjectiveCompleted(SRGameplayTags::GameFlow_Objectives_CobmineAudio);
		}
		//ClueWidget->CombineButtonClickedDelegate.RemoveDynamic(this, &ThisClass::HandleCombineClueTutorial);
	}
}

void USRClueWidgetPresenter::RequestRemoveDeviceData(FName Id)
{
	TArray<FName> Ids;
	Ids.Add(Id);
	InvenComp->RemoveItems(Ids);
}
