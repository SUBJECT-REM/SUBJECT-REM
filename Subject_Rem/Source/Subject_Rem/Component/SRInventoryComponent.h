// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRItemData.h"

#include "SRInventoryComponent.generated.h"

class USRItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeClueDatasSignatue,const FSRItemBaseData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeInventoryDataSignature, const FSRItemBaseData&, Data);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USRInventoryComponent();
	
	UFUNCTION(BlueprintCallable)
	void AddItem(const USRItem* Item);

	UFUNCTION()
	void CombineClue(TArray<FName> ClueIds);

	/*ClueDatas 변경에 대한 델리게이트*/
	FChangeClueDatasSignatue ChangeClueDatasDelegate;

	FChangeInventoryDataSignature ChangeInventoryDataDelegate;

	UPROPERTY(VisibleAnywhere)
	TArray<const USRItem*> InventoryItems;

private:	
	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRItemBaseData& Data);

	UFUNCTION(BlueprintCallable)
	void AddItemData(const FSRItemData& Data);

	/*단서 조합을 위한 데이터 테이블*/
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ClueCombineDataRuleTable;

	const int first = 0;
	const int second = 1;
};
