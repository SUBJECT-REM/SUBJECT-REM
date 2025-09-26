// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SRInventoryComponent.h"
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

void USRInventoryComponent::AddItem(const USRItem* Item)
{
	check(Item);

	const IUseableInterface* UseableItem = Cast<IUseableInterface>(Item);
	const FSRItemData ItemData = Item->GetItemData();

	FSRDeviceItemData DeviceRow;
	if (TryGetDeviceRow(ItemData, DeviceRow))
	{
		//Device데이터임.

		FSRDeviceUIData UIData;
		UIData.Base = ItemData.BaseInfo;
		UIData.UsingSlotNum = FMath::Clamp<int32>(DeviceRow.UsingClueNum, 1, 3);

		AddDeviceData(UIData);
	}
	else if (UseableItem)
	{

	}
	//Useable아이템이 아니라면. ClueData입니다, Device일수도있음.
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
    if (!ClueCombineRuleDataTable || ClueIds.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("조합 실패: 잘못된 단서 개수 또는 테이블 없음"));
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
    FSRClueMapData* FoundClueMap = nullptr;

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
        if (!R.DataTable) { UE_LOG(LogTemp, Warning, TEXT("ClueMapRow.DataTable nullptr")); return; }

        FString Ctx(TEXT("ClueMap"));
        FoundClueMap = R.DataTable->FindRow<FSRClueMapData>(R.RowName, Ctx);
        if (!FoundClueMap) { UE_LOG(LogTemp, Warning, TEXT("ClueMapData 찾기 실패: %s"), *R.RowName.ToString()); return; }

        MatchedRule = Rule;
        break;
    }

    if (!MatchedRule || !FoundClueMap)
    {
        UE_LOG(LogTemp, Warning, TEXT("일치하는(순서 민감) 조합 룰이 없음"));
        return;
    }

    // 진실만 누적(기존 로직 유지)
    if (FoundClueMap->bResult)
    {
        ClueMapDatas.Add(*FoundClueMap);
    }

    // 재료 제거 (입력 순서 그대로)
    RemoveItems(ClueIds);

    // ---- UI 페이로드 구성 & 방송 (요지는 동일, 생략 가능) ----
    FSRClueMapUIData Payload;
    Payload.ClueMap = *FoundClueMap;
    Payload.ClueIds = ClueIds; // 순서 보존
	Payload.bResult = FoundClueMap->bResult;
    if (AllItemsDataTable)
    {
        // AllItemData에서 이름/아이콘 조회
        for (const FName& Id : ClueIds)
        {
            FString FindClueCtx;
            FSRItemData* FindClue = AllItemsDataTable->FindRow<FSRItemData>(Id, FindClueCtx);

            if (FindClue)
            {
                Payload.ClueNames.Add(FindClue->BaseInfo.Name);
                Payload.ClueIcons.Add(FindClue->BaseInfo.Icon);
                
            }

        }
    }

    ClueCombineResultDelegate.Broadcast(Payload);

	//아이템 조합 후 표출될 자막이 있다면
	if (FoundClueMap->bShowCaption)
	{
		ClueCombineCaptionDelegate.Broadcast(FoundClueMap->CaptionRow.RowName);
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
	float ImmediateSum = 0.f;
	struct FPeriodic { float Amount = 0.f; float Interval = 0.f; };
	TArray<FPeriodic> Periodics;

	for (const FSRClueMapData& Data : ClueMapDatas)
	{
		if (!Data.bResult) continue; // 거짓 결과는 무시
		ImmediateSum += Data.ImmediateStessIncrease; // (필드명 오타 유지시 그대로 사용)

		if (Data.PeriodicStressIncrease.Amount != 0.f &&
			Data.PeriodicStressIncrease.Interval > 0.f)
		{
			Periodics.Add({ Data.PeriodicStressIncrease.Amount,
							Data.PeriodicStressIncrease.Interval });
		}
	}

	if (ImmediateSum != 0.f)
	{
		CashedStressSubsystem->ChangeStressAmount(ImmediateSum);
	}
	for (const auto& P : Periodics)
	{
		CashedStressSubsystem->ChangeStressByTime(P.Amount, P.Interval);
	}

	// 같은 조합 결과가 다음에 또 중복 적용되지 않도록 초기화
	ClueMapDatas.Reset();
}
