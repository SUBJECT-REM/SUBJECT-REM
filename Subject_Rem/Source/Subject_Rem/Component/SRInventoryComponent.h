// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRItemData.h"

#include "SRInventoryComponent.generated.h"

class USRItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeClueDatasSignatue,const FSRItemBaseData&, Data);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECT_REM_API USRInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USRInventoryComponent();
	
	/*단서 데이터 추가*/
	UFUNCTION(BlueprintCallable)
	void AddClueData(const FSRItemBaseData& Data);

	UFUNCTION(BlueprintCallable)
	void AddItemData(const FSRItemData& Data);

	UFUNCTION(BlueprintCallable)
	void AddItem(const USRItem* Item);

	UFUNCTION()
	void CombineClue(TArray<FName> ClueIds);

	/*ClueDatas 변경에 대한 델리게이트*/
	FChangeClueDatasSignatue ChangeClueDatasDelegate;

	/*소유하고 있는 단서 데이터들*/
	UPROPERTY(VisibleAnywhere)
	TArray<FSRItemBaseData> ClueDatas;

	UPROPERTY(VisibleAnywhere)
	TArray<USRItem*> AllItem;

private:	

		
	/*단서 조합을 위한 데이터 테이블*/
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ClueCombineDataTable;

	/*단서 조합을 위한 데이터 테이블*/
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ClueDataTable;

	const int first = 0;
	const int second = 1;
};
