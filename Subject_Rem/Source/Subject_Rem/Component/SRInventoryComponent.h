// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRItemData.h"

#include "SRInventoryComponent.generated.h"

class UDataTable;
class USRItem;
class USRItemPickupResultPresenter;
class USRStressLocalPlayerSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddDeviceDataSignature, const FSRDeviceUIData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddClueDatasSignatue,const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddInventoryDataSignature, const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveInventoryDataSignature, const TArray<FName>&, RemovedItemIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueCombineResultSignature, const FSRClueCombineResultUIData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueMapResultSignature, const FSRClueMapUIData&, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueCombineCaptionSignature, const FName& ,CaptionRowName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpSignature, const FSRItemBaseData&, Base);


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

	void FlushStressFromClueMaps();

	UFUNCTION()
	uint8 GetTrueClueMapData();

	/*Clue 변경에 대한 델리게이트*/
	FAddClueDatasSignatue AddClueDatasDelegate;
	FAddDeviceDataSignature AddDeviceDataDelegate;
	/*인벤토리 아이템 추가,제거에 대한 델리게이트*/
	FAddInventoryDataSignature AddInventoryDataDelegate;
	FRemoveInventoryDataSignature RemoveInventoryDataDelegate;

	/*ClueMap 생성에 대한 델리게이트*/
	FOnClueCombineResultSignature ClueCombineResultDelegate;
	FOnClueCombineCaptionSignature ClueCombineCaptionDelegate;
	FOnClueMapResultSignature ClueMapResultDelegate;

	FOnItemPickedUpSignature ItemPickupDelegate;

	UPROPERTY(VisibleAnywhere)
	TArray<const USRItem*> InventoryItems;

	//조합 결과의 모음 - > 옮겨도 괜춘함.
	UPROPERTY(VisibleAnywhere)
	TArray<FSRClueMapData> ClueMapDatas;
protected:
	virtual void BeginPlay() override;
private:	
	void ApplyFallbackFalseClue(const TArray<FName>& ConsumedIds);

	// 1) 룰과 입력 매칭 여부 판단
	bool DoesRuleMatchInput(const TArray<FName>& InputRaw,
							const TArray<FName>& InputNormalized,
							const struct FSRClueCombineRuleData* Rule) const;

	// 2) 결과 RowHandle을 해석해서 적용(ClueMap/Item) — 적용 성공 시 true 반환
	bool TryApplyCombineResult(const FDataTableRowHandle& Handle,
							   const TArray<FName>& ConsumedIds);

	// 3) 결과 적용(ClueMap / Item)
	void ApplyClueMapResult(const struct FSRClueMapData& ClueMap,
							const TArray<FName>& ConsumedIds);

	void ApplyItemResult(const struct FSRItemData& NewItem,
						 const TArray<FName>& ConsumedIds);

	// 4) UI용 이름/아이콘 채우기 유틸
	void FillNamesAndIcons(const TArray<FName>& Ids,
						   struct FSRClueMapUIData& InOutPayload) const;

	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRItemBaseData& Data);

	UFUNCTION(BlueprintCallable)
	void AddDeviceData(const FSRDeviceUIData& Data);


	UFUNCTION(BlueprintCallable)
	void AddItemData(const FSRItemData& Data);

	//아이템 줍기시 나타낼 위젯에 대한 Presenter를 생성 및 초기화합니다.
	//아이템 줍을 시점 및 데이터와 밀접하여 InventoryComp에 넣어놨음.
	void EnsureItemPickupPresenter();

	bool TryGetDeviceRow(const FSRItemData& ItemData, FSRDeviceItemData& OutDeviceRow) const;

	bool TryAutoRegisterToQuickSlot(const FSRItemData& ItemData);

	bool IsConsumable(const FSRItemData& ItemData) const;

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

	TWeakObjectPtr<USRStressLocalPlayerSubsystem> CashedStressSubsystem;
	TWeakObjectPtr<class ASRGameFlowManager> CashedGameFlowMng;
	const int first = 0;
	const int second = 1;
};
