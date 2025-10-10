// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRQuickSlotComponent.generated.h"

class UTextrue2D;
class UDataTable;
struct FSRConsumeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotChangedSignature, int32, SlotIndex,  TSoftObjectPtr<UTexture2D> , Icon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SUBJECT_REM_API USRQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRQuickSlotComponent();
	//퀵 슬롯 입력을 받아옵니다.
	void PressQuickSlot(uint8 QuickSlotNum);

	bool RegisterItem(uint8 Index, FName Id);

	void UnRegisterItem(uint8 Index);

	bool CanRegisterItem(FName Id);

	UFUNCTION(BlueprintCallable)
	bool TryUseRSlot(AActor* Target);

	bool TryAutoRegisterItem(FName ItemId);
	
	FName GetItemIdBySlotIndex(uint8 Index);

	const FSRConsumeData* ResolveConsumeDataByItemId(FName ItemId) const;

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UTexture2D> GetSlotIconByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void GetQuickslotSnapshot(TArray<TSoftObjectPtr<UTexture2D>>& OutIcons) const;

	FOnQuickSlotChangedSignature OnQuickSlotChangedDelegate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//실제 QuickSlot에 있는 아이템을 들고와서 사용합니다.
	virtual void UseQuickSlotItem(uint8 QuickSlotNum);
public:	

private:
	UPROPERTY()
	TArray<FName> Slots;

	const uint8 RSlotIndex = 3;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDataTableRowHandle> StartQuickItemDatas;

	UPROPERTY() 
	TArray<TSoftObjectPtr<UTexture2D>> SlotIcons; // 아이콘 캐시

	TSoftObjectPtr<UTexture2D> ResolveIconByItemId(FName Id) const; // DT에서 찾아 반환

	TSoftObjectPtr<class ASRGameFlowManager> GameFlowManager;

};
