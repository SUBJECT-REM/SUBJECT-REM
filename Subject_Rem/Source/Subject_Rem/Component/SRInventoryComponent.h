// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRItemData.h"

#include "SRInventoryComponent.generated.h"

class UDataTable;
class USRItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddClueDatasSignatue,const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddInventoryDataSignature, const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveInventoryDataSignature, const TArray<FName>&, RemovedItemIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueMapResultSignature, const FSRItemBaseData&, Data);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USRInventoryComponent();
	
	UFUNCTION(BlueprintCallable)
	void AddItem(const USRItem* Item);

	UFUNCTION()
	void RemoveItems(const TArray<FName>& ItemIds);

	UFUNCTION()
	void CombineClue(TArray<FName> ClueIds);

	/*Clue 변경에 대한 델리게이트*/
	FAddClueDatasSignatue AddClueDatasDelegate;

	/*인벤토리 아이템 추가,제거에 대한 델리게이트*/
	FAddInventoryDataSignature AddInventoryDataDelegate;
	FRemoveInventoryDataSignature RemoveInventoryDataDelegate;

	/*ClueMap 생성에 대한 델리게이트*/
	FOnClueMapResultSignature ClueMapResultDelegate;

	UPROPERTY(VisibleAnywhere)
	TArray<const USRItem*> InventoryItems;

	UPROPERTY(VisibleAnywhere)
	TArray<FSRClueMapData> ClueMapDatas;
private:	
	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRItemBaseData& Data);

	UFUNCTION(BlueprintCallable)
	void AddItemData(const FSRItemData& Data);

	UPROPERTY(EditDefaultsOnly)
	UDataTable* AllItemsDataTable;
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ClueCombineRuleDataTable;

	const int first = 0;
	const int second = 1;
};
