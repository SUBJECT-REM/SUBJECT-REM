// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
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
	AddClueDatasDelegate.Broadcast(Data);
}

void USRInventoryComponent::AddItemData(const FSRItemData& Data)
{
	AddInventoryDataDelegate.Broadcast(Data.BaseInfo);
}

void USRInventoryComponent::AddItem(const USRItem* Item)
{
	check(Item);

	const IUseableInterface* UseableItem = Cast<IUseableInterface>(Item);
	const FSRItemData ItemData = Item->GetItemData();
	

	if (UseableItem)
	{

	}
	//Useable아이템이 아니라면. ClueData입니다.
	else
	{
		AddClueData(ItemData.BaseInfo);
	}

	AddItemData(ItemData);
	InventoryItems.Add(Item);

}

void USRInventoryComponent::RemoveItems(const TArray<FName>& ItemIds)
{
	// 기존 단서 제거
	InventoryItems.RemoveAll([&](const USRItem* Item)
	{
	if (!Item) return false;
	const FSRItemData& ItemData = Item->GetItemData();
	return ItemIds.Contains(ItemData.BaseInfo.Id);
	});

	RemoveInventoryDataDelegate.Broadcast(ItemIds);

}

void USRInventoryComponent::CombineClue(TArray<FName> ClueIds)
{
	if (!ClueCombineRuleDataTable || ClueIds.Num() != 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("조합 실패: 잘못된 단서 개수 또는 테이블 없음"));
		return;
	}

	const FName ClueA = ClueIds[0];
	const FName ClueB = ClueIds[1];

	// 테이블 순회
	for (const auto& Row : ClueCombineRuleDataTable->GetRowMap())
	{
		const FSRClueCombineRuleData* CombineRuleData = reinterpret_cast<FSRClueCombineRuleData*>(Row.Value);
		if (!CombineRuleData) continue;

		const bool bMatch =
			(CombineRuleData->ClueId1 == ClueA && CombineRuleData->ClueId2 == ClueB);

		if (bMatch)
		{
			// 결과 Row 접근
			FString ClueMapContext;
			const FDataTableRowHandle& ClueMapRow = CombineRuleData->ClueCombineResult;
			FName ClueCombineResultRowName = ClueMapRow.RowName;
			const UDataTable* ClueMapDataTable = ClueMapRow.DataTable;

			if (!ClueMapDataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClueCombineRule RowName :%s"), *ClueCombineResultRowName.ToString());
				UE_LOG(LogTemp, Warning, TEXT("ClueCombineRule : ClueMapRow Data Table nullptr"));
				return;
			}

			FSRClueMapData* FindClueMapResult = ClueMapDataTable->FindRow<FSRClueMapData>(ClueCombineResultRowName, ClueMapContext);
			if (FindClueMapResult->bResult)
			{
				ClueMapDatas.Add(*FindClueMapResult);
			}
		
			RemoveItems(ClueIds);

			//UI 델리게이트 브로드캐스트
			ClueMapResultDelegate.Broadcast(FindClueMapResult->BaseInfo);
			return;
		}
	}
}



