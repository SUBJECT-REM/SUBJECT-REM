// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
#include "Component/Character/SRQuickSlotComponent.h"
#include "SRItem.h"
#include "Interface/UseableInterface.h"
#include "Presenter/SRItemPickupResultPresenter.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Manager/SRGameFlowManager.h"
#include "SRFunctionLibrary.h"
#include "SRGameplayTags.h"

// Sets default values for this component's properties
USRInventoryComponent::USRInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USRInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	EnsureItemPickupPresenter();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	if (OwnerPawn->IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				CashedStressSubsystem = LP->GetSubsystem<USRStressLocalPlayerSubsystem>();
			}
		}
	}
	
	//CashedGameFlowMng = UGameplayStatics::GetActorOfClass(GetWorld(), ASRGameFlowManager::StaticClass());


}

void USRInventoryComponent::ApplyFallbackFalseClue(const TArray<FName>& ConsumedIds)
{
	// 기본 뼈대
	FSRClueMapData Out;
	Out.Id = USRFunctionLibrary::MakeFalseClueIdFrom(ConsumedIds);
	Out.Name = FText::GetEmpty();
	Out.bResult = false;
	Out.ImmediateStessIncrease = 0.f;
	Out.PeriodicStressIncrease = {}; // 0 초기화
	Out.bShowCaption = false;
	Out.CaptionRow = FDataTableRowHandle{};

	// 룬 설명 생성
	const int32 Seed = USRFunctionLibrary::MakeSeedFromIds(ConsumedIds);
	const int32 TargetLen = 48; // 고정 길이(원하면 24~96 등 범위로 랜덤)
	Out.Description = FText::FromString(USRFunctionLibrary::MakeRuneGibberish(TargetLen, Seed));

	// 공통 적용
	ApplyClueMapResult(Out, ConsumedIds);
}

bool USRInventoryComponent::DoesRuleMatchInput(const TArray<FName>& InputRaw, const TArray<FName>& InputNormalized, const FSRClueCombineRuleData* Rule) const
{
	if (!Rule) return false;

	const TArray<FName>& RuleIds = Rule->ClueIds;

	if (RuleIds.Num() != InputRaw.Num())
		return false;

	if (RuleIds.Num() == 3) // 3개는 순서 민감
	{
		return (RuleIds == InputRaw);
	}
	else                    // 그 외(2개 등)는 순서 무시
	{
		TArray<FName> NormalizedRule = RuleIds;
		USRFunctionLibrary::NormalizeIds(NormalizedRule);
		return (NormalizedRule == InputNormalized);
	}
}

bool USRInventoryComponent::TryApplyCombineResult(const FDataTableRowHandle& Handle, const TArray<FName>& ConsumedIds)
{
	if (!Handle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClueMapRow.DataTable nullptr"));
		return false;
	}

	const UScriptStruct* RowStruct = Handle.DataTable->GetRowStruct();
	FString Ctx(TEXT("CombineResult"));

	if (RowStruct == FSRClueMapData::StaticStruct())
	{
		const FSRClueMapData* FoundClueMap =
			Handle.DataTable->FindRow<FSRClueMapData>(Handle.RowName, Ctx);

		if (!FoundClueMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueMapData 찾기 실패: %s"), *Handle.RowName.ToString());
			return false;
		}

		FSRClueMapData Out = *FoundClueMap;

		ApplyClueMapResult(Out, ConsumedIds);
		return true;
	}
	else if (RowStruct == FSRItemData::StaticStruct())
	{
		const FSRItemData* NewItem =
			Handle.DataTable->FindRow<FSRItemData>(Handle.RowName, Ctx);

		if (!NewItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Clue(아이템) Row 찾기 실패: %s"), *Handle.RowName.ToString());
			return false;
		}

		ApplyItemResult(*NewItem, ConsumedIds);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("지원하지 않는 결과 RowStruct: %s"),
			   *GetNameSafe(RowStruct));
		return false;
	}
}

void USRInventoryComponent::ApplyClueMapResult(const FSRClueMapData& ClueMap, const TArray<FName>& ConsumedIds)
{
	// 인벤토리 데이터 반영
	ClueMapDatas.Add(ClueMap);
	if (ClueMap.bResult)
	{
		NumOfTrueClueMap++
	}

	// ClueMap 업데이트 페이로드
	FSRClueMapUIData ClueMapPayload;
	ClueMapPayload.ClueMap = ClueMap;
	ClueMapPayload.bResult = ClueMap.bResult;
	ClueMapPayload.ClueIds = ConsumedIds;

	// UI 페이로드
	FSRClueCombineResultUIData CombineResultPayload;
	CombineResultPayload.ClueMap.Description = ClueMap.Description;
	CombineResultPayload.ClueMap.Name = ClueMap.Name;
	for (const FName& Id : ConsumedIds)
	{
		FString FindCtx;
		if (FSRItemData* Find = AllItemsDataTable->FindRow<FSRItemData>(Id, FindCtx))
		{
			CombineResultPayload.ClueIcons.Add(Find->BaseInfo.Icon);
		}
	}

	CombineResultPayload.ClueIds = ConsumedIds;

	FillNamesAndIcons(ConsumedIds, ClueMapPayload);
	// 브로드캐스트
	ClueCombineResultDelegate.Broadcast(CombineResultPayload);

	ClueMapResultDelegate.Broadcast(ClueMapPayload);

	if (ClueMap.bShowCaption)
	{
		ClueCombineCaptionDelegate.Broadcast(ClueMap.CaptionRow.RowName);
	}

}

void USRInventoryComponent::ApplyItemResult(const FSRItemData& NewItem, const TArray<FName>& ConsumedIds)
{
	// 재료 제거 → 신규 아이템 추가
	RemoveItems(ConsumedIds);
	AddClueData(NewItem.BaseInfo);
	AddItemData(NewItem);

	// 조합 결과 UI 페이로드
	FSRClueCombineResultUIData Payload;
	Payload.ClueMap.Description = NewItem.BaseInfo.Description;
	Payload.ClueMap.Name = NewItem.BaseInfo.Name;
	Payload.ClueIcons.Add(NewItem.BaseInfo.Icon);
	Payload.ClueIds = ConsumedIds;

	ClueCombineResultDelegate.Broadcast(Payload);
}

void USRInventoryComponent::FillNamesAndIcons(const TArray<FName>& Ids, FSRClueMapUIData& InOutPayload) const
{
	if (!AllItemsDataTable) return;

	for (const FName& Id : Ids)
	{
		FString FindCtx;
		if (FSRItemData* Find = AllItemsDataTable->FindRow<FSRItemData>(Id, FindCtx))
		{
			InOutPayload.ClueNames.Add(Find->BaseInfo.Name);
			InOutPayload.ClueIcons.Add(Find->BaseInfo.Icon);
		}
	}
}

void USRInventoryComponent::AddClueData(const FSRItemBaseData& Data)
{
	AddClueDatasDelegate.Broadcast(Data);
}

void USRInventoryComponent::AddDeviceData(const FSRDeviceUIData& Data)
{
	AddDeviceDataDelegate.Broadcast(Data);
}

void USRInventoryComponent::AddItemData(const FSRItemData& Data)
{
	AddInventoryDataDelegate.Broadcast(Data.BaseInfo);
}

//TODO : PickUp 관련 따로 분리
void USRInventoryComponent::EnsureItemPickupPresenter()
{
	if (!ItemPickupPresenter)
	{
		ItemPickupPresenter = NewObject<USRItemPickupResultPresenter>(this, ItemPickupPresenterClass);

		check(ItemPickupPresenter);
		ItemPickupPresenter->Init(this, nullptr);
	}
}

bool USRInventoryComponent::TryGetDeviceRow(const FSRItemData& ItemData, FSRDeviceItemData& OutDeviceRow) const
{
	const FDataTableRowHandle& Handle = ItemData.ItemDataTable;
	const UDataTable* Table = Handle.DataTable;
	if (!Table) return false;

	const UScriptStruct* RowStruct = Table->GetRowStruct();
	if (RowStruct != FSRDeviceItemData::StaticStruct())
	{
		// 이 테이블은 디바이스 테이블이 아님
		return false;
	}

	FString Ctx(TEXT("DeviceRowLookup"));
	if (const FSRDeviceItemData* Row = Table->FindRow<FSRDeviceItemData>(Handle.RowName, Ctx))
	{
		OutDeviceRow = *Row; // 복사
		return true;
	}
	
	return false;
}

bool USRInventoryComponent::TryAutoRegisterToQuickSlot(const FSRItemData& ItemData)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;

	if (USRQuickSlotComponent* Quick = OwnerPawn->FindComponentByClass<USRQuickSlotComponent>())
	{
		// 인덱스 결정/등록은 전부 QuickSlot 쪽에서
		const bool bOk = Quick->TryAutoRegisterItem(ItemData.BaseInfo.Id );
		if (bOk)
		{
			UE_LOG(LogTemp, Log, TEXT("[Inven] Auto-registered to QuickSlot: %s"), *ItemData.BaseInfo.Id.ToString());
		}
		return bOk;
	}
	return false;
}


void USRInventoryComponent::AddItem(const USRItem* Item)
{
	check(Item);

	const FSRItemData ItemData = Item->GetItemData();
	ItemPickupDelegate.Broadcast(ItemData.BaseInfo);
	bool IsItemConsume = false;
	FSRDeviceItemData DeviceRow;
	if (TryGetDeviceRow(ItemData, DeviceRow))
	{
		//Device데이터임.

		FSRDeviceUIData UIData;
		UIData.Base = ItemData.BaseInfo;
		UIData.UsingSlotNum = FMath::Clamp<int32>(DeviceRow.UsingClueNum, 1, 3);
		UIData.AllowedItemIds = DeviceRow.AllowedItemIds;
		AddDeviceData(UIData);
	}
	else if (TryAutoRegisterToQuickSlot(ItemData))
	{
		return;
	}
	else
	{
		if (!IsConsumable(ItemData))     
		{
			AddClueData(ItemData.BaseInfo);
		}
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
	if (!ClueCombineRuleDataTable) return;

	// 입력 정리
	ClueIds.RemoveAll([](const FName& N) { return N.IsNone(); });
	if (ClueIds.Num() == 0) return;

	TArray<FName> NormalizedInput = ClueIds;
	USRFunctionLibrary::NormalizeIds(NormalizedInput);

	// 룰 매칭 루프
	for (const auto& Pair : ClueCombineRuleDataTable->GetRowMap())
	{
		const FSRClueCombineRuleData* Rule = reinterpret_cast<const FSRClueCombineRuleData*>(Pair.Value);
		if (!Rule) continue;

		if (!DoesRuleMatchInput(ClueIds, NormalizedInput, Rule))
			continue;

		// 결과 RowHandle 적용 시도
		const FDataTableRowHandle& Handle = Rule->ClueCombineResult;
		if (TryApplyCombineResult(Handle, ClueIds))
		{
			RemoveItems(ClueIds);
			return; // 첫 매칭 처리 후 종료
		}
	}

	
	//CashedGameFlowMng->NotifyObjectiveCompleted(SRGameplayTags::Tutorial_Objectives_CombineClue);
	//거짓단서 처리
		// 재료 제거
	RemoveItems(ClueIds);
	ApplyFallbackFalseClue(ClueIds);
}

void USRInventoryComponent::FlushStressFromClueMaps()
{

	// ClueMapDatas 에 누적된 결과들을 한 번에 반영
	if (ClueMapDatas.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClueMapDatasNum 0"));
		return;
	}
	float Immediate = 0.f;
	struct FPeriodic { float Amount = 0.f; float Interval = 0.f; };
	FPeriodic Periodics;

	for (const FSRClueMapData& Data : ClueMapDatas)
	{
		if (!Data.bResult) continue; // 거짓 결과는 무시
		Immediate = Data.ImmediateStessIncrease; // (필드명 오타 유지시 그대로 사용)

		if (Data.PeriodicStressIncrease.Amount != 0.f &&
			Data.PeriodicStressIncrease.Interval > 0.f)
		{
			Periodics.Amount = Data.PeriodicStressIncrease.Amount;
			Periodics.Interval = Data.PeriodicStressIncrease.Interval;
		}
	}

	if (Immediate != 0.f)
	{
		CashedStressSubsystem->ChangeStressAmount(Immediate);
	}
	if (Periodics.Amount > 0.f && Periodics.Interval > 0.f)
	{
		CashedStressSubsystem->ChangeStressByTime(Periodics.Amount, Periodics.Interval);
	}

	// 같은 조합 결과가 다음에 또 중복 적용되지 않도록 초기화
	ClueMapDatas.Reset();
}

uint8 USRInventoryComponent::GetTrueClueMapData()
{
	//uint8 NumOfTrueClueMap = 0;
	//for (FSRClueMapData Data : ClueMapDatas)
	//{
	//	if (Data.bResult == true)
	//	{
	//		NumOfTrueClueMap++;
	//	}
	//}

	//return NumOfTrueClueMap;

	return NumOfTrueClueMap;
}

bool USRInventoryComponent::IsConsumable(const FSRItemData& ItemData) const
{
	const FDataTableRowHandle& Sub = ItemData.ItemDataTable;
	return (Sub.DataTable && Sub.DataTable->GetRowStruct() == FSRConsumeData::StaticStruct());
}
