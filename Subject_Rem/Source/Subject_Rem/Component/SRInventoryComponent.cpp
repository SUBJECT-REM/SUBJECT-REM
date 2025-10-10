// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
#include "Component/Character/SRQuickSlotComponent.h"
#include "SRItem.h"
#include "Interface/UseableInterface.h"
#include "Presenter/SRItemPickupResultPresenter.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"

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

void USRInventoryComponent::NormalizeIds(TArray<FName>& Arr)
{
	Arr.Sort([](const FName& L, const FName& R) {
		return L.LexicalLess(R);
 });
}

bool USRInventoryComponent::IsEqualSorted(TArray<FName> A, TArray<FName> B)
{
	if (A.Num() != B.Num()) return false;
	NormalizeIds(A);
	NormalizeIds(B);
	for (int32 i = 0; i < A.Num(); ++i)
	{
		if (A[i] != B[i]) return false;
	}
	return true;
}


void USRInventoryComponent::AddItem(const USRItem* Item)
{
	check(Item);

	const FSRItemData ItemData = Item->GetItemData();
	ItemPickupDelegate.Broadcast(ItemData.BaseInfo);

	FSRDeviceItemData DeviceRow;
	if (TryGetDeviceRow(ItemData, DeviceRow))
	{
		//Device데이터임.

		FSRDeviceUIData UIData;
		UIData.Base = ItemData.BaseInfo;
		UIData.UsingSlotNum = FMath::Clamp<int32>(DeviceRow.UsingClueNum, 1, 3);

		AddDeviceData(UIData);
	}
	else if (TryAutoRegisterToQuickSlot(ItemData))
	{
		return;
	}
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
	if (!ClueCombineRuleDataTable) return;

	// 입력 정리 + 정렬 캐시(2개 조합용)
	ClueIds.RemoveAll([](const FName& N) { return N.IsNone(); });
	if (ClueIds.Num() == 0) return;

	TArray<FName> NormalizedInput = ClueIds;
	NormalizeIds(NormalizedInput);

	for (const auto& Pair : ClueCombineRuleDataTable->GetRowMap())
	{
		const FSRClueCombineRuleData* Rule = reinterpret_cast<const FSRClueCombineRuleData*>(Pair.Value);
		if (!Rule) continue;

		// 룰 재료(가변 필드 우선, 구버전 2개 호환)
		TArray<FName> RuleIds = Rule->ClueIds;

		// 개수 불일치 스킵
		if (RuleIds.Num() != ClueIds.Num()) continue;

		// ====== 여기서 '매칭 여부' 먼저 결정 ======
		bool bMatched = false;

		if (RuleIds.Num() == 3)
		{
			// 3개 조합은 '순서 민감': 입력 그대로 일치해야 함
			bMatched = (RuleIds == ClueIds);
		}
		else
		{
			// 그 외(주로 2개)는 '순서 무시': 정렬 후 비교
			TArray<FName> NormalizedRule = RuleIds;
			NormalizeIds(NormalizedRule);
			bMatched = (NormalizedRule == NormalizedInput);
		}

		if (!bMatched) continue;
		// ========================================

		// 결과 접근
		const FDataTableRowHandle& R = Rule->ClueCombineResult;
		if (!R.DataTable) { UE_LOG(LogTemp, Warning, TEXT("ClueMapRow.DataTable nullptr")); continue; }

		const UScriptStruct* RowStruct = R.DataTable->GetRowStruct();
		FString Ctx(TEXT("CombineResult"));

		if (RowStruct == FSRClueMapData::StaticStruct())
		{
			// ===== 기존 ClueMap 처리 그대로 =====
			FSRClueMapData* FoundClueMap = R.DataTable->FindRow<FSRClueMapData>(R.RowName, Ctx);
			if (!FoundClueMap) { UE_LOG(LogTemp, Warning, TEXT("ClueMapData 찾기 실패: %s"), *R.RowName.ToString()); continue; }

			if (FoundClueMap->bResult)
			{
				ClueMapDatas.Add(*FoundClueMap);
			}

			RemoveItems(ClueIds);

			FSRClueMapUIData Payload;
			Payload.ClueMap = *FoundClueMap;
			Payload.bResult = FoundClueMap->bResult;
			Payload.ClueIds = ClueIds;

			if (AllItemsDataTable)
			{
				for (const FName& Id : ClueIds)
				{
					FString FindCtx;
					if (FSRItemData* Find = AllItemsDataTable->FindRow<FSRItemData>(Id, FindCtx))
					{
						Payload.ClueNames.Add(Find->BaseInfo.Name);
						Payload.ClueIcons.Add(Find->BaseInfo.Icon);
					}
				}
			}

			ClueCombineResultDelegate.Broadcast(Payload);

			if (FoundClueMap->bShowCaption)
			{
				ClueCombineCaptionDelegate.Broadcast(FoundClueMap->CaptionRow.RowName);
			}

			return; // 첫 매칭 처리 후 종료
		}
		else if (RowStruct == FSRItemData::StaticStruct())
		{
			// ===== 기존 새 아이템 처리 그대로 =====
			const FSRItemData* NewClue = R.DataTable->FindRow<FSRItemData>(R.RowName, Ctx);
			if (!NewClue) { UE_LOG(LogTemp, Warning, TEXT("Clue(아이템) Row 찾기 실패: %s"), *R.RowName.ToString()); continue; }

			RemoveItems(ClueIds);

			AddClueData(NewClue->BaseInfo);
			AddItemData(*NewClue);

			FSRClueMapUIData Payload;
			Payload.ClueMap.Description = NewClue->BaseInfo.Description;
			Payload.ClueMap.Name = NewClue->BaseInfo.Name;
			Payload.ClueIcons.Add(NewClue->BaseInfo.Icon);
			Payload.ClueIds = ClueIds;

			ClueCombineResultDelegate.Broadcast(Payload);

			return; // 첫 매칭 처리 후 종료
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("지원하지 않는 결과 RowStruct: %s"), *GetNameSafe(RowStruct));
			continue;
		}
	}

	UE_LOG(LogTemp, Verbose, TEXT("No matching combine rule for [%s]"),
		*FString::JoinBy(ClueIds, TEXT(","), [](const FName& N) { return N.ToString(); }));

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
