// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
#include "SRClueCombineData.h"
// Sets default values for this component's properties
USRInventoryComponent::USRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USRInventoryComponent::AddClueData(const FSRClueData& Data)
{
	ClueDatas.Add(Data);
	ChangeClueDatasDelegate.Broadcast(Data);
}

void USRInventoryComponent::CombineClue(TArray<FName> ClueIds)
{
	//ClueCobine 수행
	check(ClueCombineDataTable);
	FString ClueCombineContext;
	FString ClueContext;
	FSRClueCombineData* FindCombineData =ClueCombineDataTable->FindRow<FSRClueCombineData>(ClueIds[first], ClueCombineContext);
	if (FindCombineData)
	{
		FSRClueData* CombineClueData =ClueDataTable->FindRow<FSRClueData>(FindCombineData->ReseultClueId, ClueContext);
		AddClueData(*CombineClueData);
	}

	//사용한 Clue 제거
	ClueDatas.RemoveAll([&](const FSRClueData& Data)
	{
		return ClueIds.Contains(Data.Id);
	});
}



