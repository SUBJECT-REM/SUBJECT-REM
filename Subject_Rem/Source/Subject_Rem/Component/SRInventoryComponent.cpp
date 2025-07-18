// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
#include "SRClueCombineData.h"
#include "SRItem.h"
#include "Interface/UseableInterface.h"

// Sets default values for this component's properties
USRInventoryComponent::USRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USRInventoryComponent::AddClueData(const FSRItemBaseData& Data)
{
	ClueDatas.Add(Data);
	ChangeClueDatasDelegate.Broadcast(Data);
}

void USRInventoryComponent::AddItemData(const FSRItemData& Data)
{
	FString ItemDataContext;

	//if()
	const UDataTable* DataTable = Data.ItemDataTable.DataTable;
	DataTable->FindRow<FSRItemData>(Data.ItemDataTable.RowName, ItemDataContext);
}

void USRInventoryComponent::AddItem(const USRItem* Item)
{

	const IUseableInterface* UseableItem = Cast<IUseableInterface>(Item);
	const FSRItemData ItemData = Item->GetItemData();
	
	if (UseableItem)
	{

	}

	//Useable아이템이 아니라면. ClueData입니다.
	else
	{
		const UDataTable* LocalClueDataTable = ItemData.ItemDataTable.DataTable;
		check(LocalClueDataTable)

		FString ClueDataContext;
		const FSRClueData* ClueData = LocalClueDataTable->FindRow<FSRClueData>(ItemData.ItemDataTable.RowName, ClueDataContext);

		//기본정보 업데이트
		AddClueData(ItemData.BaseInfo);
	}


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
		FSRItemBaseData* CombineClueData =ClueDataTable->FindRow<FSRItemBaseData>(FindCombineData->ReseultClueId, ClueContext);
		AddClueData(*CombineClueData);
	}

	//사용한 Clue 제거
	ClueDatas.RemoveAll([&](const FSRItemBaseData& Data)
	{
		return ClueIds.Contains(Data.Id);
	});
}



