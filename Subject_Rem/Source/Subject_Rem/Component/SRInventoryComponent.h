// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRItemData.h"

#include "SRInventoryComponent.generated.h"

class UDataTable;
class USRItem;
class USRItemPickupResultPresenter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddClueDatasSignatue,const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddInventoryDataSignature, const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveInventoryDataSignature, const TArray<FName>&, RemovedItemIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueCombineResultSignature, const FSRClueMapData&, Data);

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
	FOnClueCombineResultSignature ClueCombineResultDelegate;

	UPROPERTY(VisibleAnywhere)
	TArray<const USRItem*> InventoryItems;

	//조합 결과의 모음 - > 옮겨도 괜춘함.
	UPROPERTY(VisibleAnywhere)
	TArray<FSRClueMapData> ClueMapDatas;
protected:
	virtual void BeginPlay() override;
private:	
	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRItemBaseData& Data);

	UFUNCTION(BlueprintCallable)
	void AddItemData(const FSRItemData& Data);

	//아이템 줍기시 나타낼 위젯에 대한 Presenter를 생성 및 초기화합니다.
	//아이템 줍을 시점 및 데이터와 밀접하여 InventoryComp에 넣어놨음.
	void EnsureItemPickupPresenter();

	UPROPERTY(EditDefaultsOnly)
	UDataTable* AllItemsDataTable;
	
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ClueCombineRuleDataTable;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDataTableRowHandle> StartingItemDatas;


	UPROPERTY()
	USRItemPickupResultPresenter* ItemPickupPresenter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USRItemPickupResultPresenter> ItemPickupPresenterClass;
	const int first = 0;
	const int second = 1;
};
