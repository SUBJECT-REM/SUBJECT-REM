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
    if (!ClueCombineRuleDataTable)
    {
		return;
    }

    // None 제거(순서 유지)
	ClueIds.RemoveAll([](const FName& N) { return N.IsNone(); });

    // 순서 비교 람다
    auto IsOrderedEqual = [](const TArray<FName>& A, const TArray<FName>& B)
        {
            if (A.Num() != B.Num()) return false;
            for (int32 i = 0; i < A.Num(); ++i)
                if (A[i] != B[i]) return false;
            return true;
        };

    const FSRClueCombineRuleData* MatchedRule = nullptr;

    // 룰 순회: 개수 먼저 필터 → 순서 그대로 비교
    for (const auto& Pair : ClueCombineRuleDataTable->GetRowMap())
    {
        const FSRClueCombineRuleData* Rule = reinterpret_cast<const FSRClueCombineRuleData*>(Pair.Value);
        if (!Rule) continue;

        // 룰의 재료 목록(신규 필드 우선, 없으면 구버전 2개 사용)
        TArray<FName> RuleIds = Rule->ClueIds;
   
        // 1) 개수 다르면 스킵 (예: 2 vs 3)
        if (RuleIds.Num() != ClueIds.Num())
            continue;

        // 2) 순서 그대로 동일해야 매치
        if (!IsOrderedEqual(RuleIds, ClueIds))
            continue;

        // 결과 로우 접근
        const FDataTableRowHandle& R = Rule->ClueCombineResult;
        if (!R.DataTable) 
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueMapRow.DataTable nullptr"));
			return; 
		}

        // ★ 결과 테이블 타입 분기
        const UScriptStruct* RowStruct = R.DataTable->GetRowStruct();
        FString Ctx(TEXT("CombineResult"));

        if (RowStruct == FSRClueMapData::StaticStruct())
        {
            // ===== 기존 ClueMap 흐름 =====
            FSRClueMapData* FoundClueMap = R.DataTable->FindRow<FSRClueMapData>(R.RowName, Ctx);
            if (!FoundClueMap)
            {
                UE_LOG(LogTemp, Warning, TEXT("ClueMapData 찾기 실패: %s"), *R.RowName.ToString());
                return;
            }

            if (FoundClueMap->bResult)
            {
                ClueMapDatas.Add(*FoundClueMap);
            }

            // 재료 제거
            RemoveItems(ClueIds);
			
            // UI 페이로드 방송 (기존 유지)
            FSRClueMapUIData Payload;
			if (FoundClueMap->bResult)
			{
				Payload.ClueMap = *FoundClueMap;
				Payload.ClueIds = ClueIds;
				Payload.bResult = FoundClueMap->bResult;
			}
			else
			{
				Payload.ClueMap = *FoundClueMap;
				Payload.bResult = FoundClueMap->bResult;
			}
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
        }
        else if (RowStruct == FSRItemData::StaticStruct())
        {
            // ===== 조합 결과가 "새로운 단서(아이템)"인 경우 =====
            const FSRItemData* NewClue = R.DataTable->FindRow<FSRItemData>(R.RowName, Ctx);
            if (!NewClue)
            {
                UE_LOG(LogTemp, Warning, TEXT("Clue(아이템) Row 찾기 실패: %s"), *R.RowName.ToString());
                return;
            }

            // 재료 제거
            RemoveItems(ClueIds);

            // 인벤토리/UI 반영 (프로젝트 정책에 맞게)
            // - 단서 목록 UI
            AddClueData(NewClue->BaseInfo);
            // - 인벤토리 패널(전체 아이템 목록 UI)
            AddItemData(*NewClue);
            // - 내부 보관(USRItem 오브젝트를 꼭 생성해야 한다면 여기에 생성/추가)
            //   ex) SpawnObject<USRItemSubclass> or 팩토리 함수를 통해 USRItem 생성 후 InventoryItems.Add()


			// UI 페이로드 방송 (기존 유지)
			FSRClueMapUIData Payload;
			Payload.ClueMap.Description = NewClue->BaseInfo.Description;
			Payload.ClueMap.Name = NewClue->BaseInfo.Name;
			Payload.ClueIcons.Add(NewClue->BaseInfo.Icon);
			Payload.ClueIds = ClueIds;

			ClueCombineResultDelegate.Broadcast(Payload);

             //(선택) 조합 결과 전용 알림이 필요하면 새 델리게이트를 만들어 방송
             //OnClueCombineProducedItem.Broadcast(NewClue->BaseInfo);

            // (선택) 결과 아이템이 캡션을 동반한다면, FSRItemData에 플래그/Row를 추가해 여기서 호출
            // if (NewClue->bShowCaption) { ClueCombineCaptionDelegate.Broadcast(NewClue->CaptionRow.RowName); }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("지원하지 않는 결과 RowStruct: %s"), *GetNameSafe(RowStruct));
            return;
        }
    }

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
