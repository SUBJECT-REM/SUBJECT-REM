// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/SRQuickSlotComponent.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "SRItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Manager/SRGameFlowManager.h"
#include "Interface/SRKeyReceivable.h"

// Sets default values for this component's properties
USRQuickSlotComponent::USRQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	//1,2,3,R
	Slots.Init(NAME_None, 4);
	SlotIcons.Init(nullptr, 4); 
}

// Called when the game starts
void USRQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 GameFlowManager 액터 찾기
	if (UWorld* World = GetWorld())
	{
		GameFlowManager = Cast<ASRGameFlowManager>(
			UGameplayStatics::GetActorOfClass(World, ASRGameFlowManager::StaticClass()));
	}
}

void USRQuickSlotComponent::PressQuickSlot(uint8 QuickSlotNum)
{
	UseQuickSlotItem(QuickSlotNum);
}

bool USRQuickSlotComponent::RegisterItem(uint8 Index, FName Id)
{

	//R 무조건 3번 슬롯으로	
    const FSRConsumeData* Consume = ResolveConsumeDataByItemId(Id);
    if (!Consume) return false;

	if (Consume->SlotType == EQuickSlotType::SpecialR)
	{
		// ★ 인덱스 3에 꽂기
		FString Ctx;
		const FSRItemData* Item = ItemDataTable->FindRow<FSRItemData>(Id, Ctx);
		Slots[RSlotIndex] = Id;
		SlotIcons[RSlotIndex] = Item ? Item->BaseInfo.Icon : nullptr;
		OnQuickSlotChangedDelegate.Broadcast(RSlotIndex, SlotIcons[RSlotIndex]);
		return true;
	}


	//일반 사용 아이템
	if (!Slots.IsValidIndex(Index) || Id.IsNone() || !ItemDataTable) return false;

	if (!CanRegisterItem(Id)) {
		UE_LOG(LogTemp, Warning, TEXT("RegisterItem failed: not a consumable (%s)"), *Id.ToString());
		return false;
	}

	FString Ctx;
	const FSRItemData* FindData = ItemDataTable->FindRow<FSRItemData>(Id, Ctx);
	if (!FindData) return false;

	Slots[Index] = Id;
	SlotIcons[Index] = FindData->BaseInfo.Icon;
	OnQuickSlotChangedDelegate.Broadcast(Index, FindData->BaseInfo.Icon);
	return true;
}

void USRQuickSlotComponent::UnRegisterItem(uint8 Index)
{
	if (!Slots.IsValidIndex(Index))
		return;

	Slots[Index] = NAME_None;
	SlotIcons[Index] = nullptr;

	OnQuickSlotChangedDelegate.Broadcast(Index, nullptr);
}

bool USRQuickSlotComponent::CanRegisterItem(FName Id)
{
	if (!ItemDataTable || Id.IsNone()) return false;

	FString Ctx;
	const FSRItemData* Item = ItemDataTable->FindRow<FSRItemData>(Id, Ctx);
	if (!Item) return false;

	const FDataTableRowHandle& Sub = Item->ItemDataTable;
	if (Sub.IsNull() || !Sub.DataTable) return false;

	return Sub.DataTable->GetRowStruct() == FSRConsumeData::StaticStruct();
}

bool USRQuickSlotComponent::TryUseRSlot(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("TryUseRSlot"));
	if (!Target) 
	{
		UE_LOG(LogTemp,Warning,TEXT("TryUseRSlot Target nullptr"))
		return  false;
	}
	const FName ItemId = GetItemIdBySlotIndex(RSlotIndex);
	if (ItemId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseRSlot : GetItemIdBySlotIndex false"));
		return false;
	}
	if (!Target->GetClass()->ImplementsInterface(USRKeyReceivable::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseRSlot : not ImplementsInterface USRKeyReceivable "));

		return false;
	}

	if (ISRKeyReceivable::Execute_CanUseKey(Target, ItemId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute_CasUseKey : true"));
		ISRKeyReceivable::Execute_UseKey(Target, ItemId, GetOwner());
		UnRegisterItem(RSlotIndex);
		return true;
	}


	UE_LOG(LogTemp, Warning, TEXT("return false"));
	return false;
}

bool USRQuickSlotComponent::TryAutoRegisterItem(FName ItemId)
{
	// 유효성 + 소비 아이템인지 확인
	const FSRConsumeData* Consume = ResolveConsumeDataByItemId(ItemId);
	if (!Consume || !Consume->AutoRegistQuickSlot)
		return false;

	// R 전용이면 3번 슬롯에
	if (Consume->SlotType == EQuickSlotType::SpecialR)
	{
		const FName Curr = GetItemIdBySlotIndex(RSlotIndex);
		if (!Curr.IsNone() )
			return false;

		return RegisterItem((uint8)RSlotIndex, ItemId);
	}

	// 기본(0~2) – 첫 빈 칸
	for (int32 i = 0; i < 3; ++i)
	{
		if (GetItemIdBySlotIndex(i).IsNone())
		{
			return RegisterItem((uint8)i, ItemId);
		}
	}


	return false;
}

FName USRQuickSlotComponent::GetItemIdBySlotIndex(uint8 Index)
{
	return (Slots.IsValidIndex(Index)) ? Slots[Index] : NAME_None;
}

const FSRConsumeData* USRQuickSlotComponent::ResolveConsumeDataByItemId(FName ItemId) const
{
	if (!ItemDataTable || ItemId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Invalid ItemDataTable or ItemId"));
		return nullptr;
	}

	FString Ctx;
	const FSRItemData* Item = ItemDataTable->FindRow<FSRItemData>(ItemId, Ctx);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Item '%s' not found in ItemDataTable"), *ItemId.ToString());
		return nullptr;
	}

	const FDataTableRowHandle& SubHandle = Item->ItemDataTable;
	if (SubHandle.IsNull() || !SubHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Item '%s' has null sub DT"), *ItemId.ToString());
		return nullptr;
	}

	// Consume 전용 테이블인지 구조 확인
	if (SubHandle.DataTable->GetRowStruct() != FSRConsumeData::StaticStruct())
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Item '%s' sub DT is not FSRConsumeData"), *ItemId.ToString());
		return nullptr;
	}

	const FSRConsumeData* Consume = SubHandle.DataTable->FindRow<FSRConsumeData>(SubHandle.RowName, Ctx);
	if (!Consume)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Consume row '%s' not found"), *SubHandle.RowName.ToString());
		return nullptr;
	}

	return Consume;

}

TSoftObjectPtr<UTexture2D> USRQuickSlotComponent::GetSlotIconByIndex(int32 Index) const
{
	return (SlotIcons.IsValidIndex(Index)) ? SlotIcons[Index] : nullptr;
}

void USRQuickSlotComponent::GetQuickslotSnapshot(TArray<TSoftObjectPtr<UTexture2D>>& OutIcons) const
{
	OutIcons = SlotIcons; // 한 번에 복사해서 반환
}


void USRQuickSlotComponent::UseQuickSlotItem(uint8 QuickSlotNum)
{
	if (QuickSlotNum == 0) 
		return;

	uint8 Index = QuickSlotNum - 1;
	
	if (!Slots.IsValidIndex(Index)) 
		return;

	const FName ItemId = GetItemIdBySlotIndex(Index);
	if (ItemId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Empty slot %d"), Index);
		return;
	}
	const FSRConsumeData* Consume = ResolveConsumeDataByItemId(ItemId);
	if (!Consume)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Not a consumable or missing consume data: %s"), *ItemId.ToString());
		return;
	}



	// 스트레스 적용 (즉시 감소)
	if (ULocalPlayer* LP = GetWorld()->GetFirstLocalPlayerFromController())
	{
		if (USRStressLocalPlayerSubsystem* StressSS = LP->GetSubsystem<USRStressLocalPlayerSubsystem>())
		{
			if (StressSS->GetStress() <= 0.f && Consume->ImmediateStessDecrease >0.f)
			{
				UE_LOG(LogTemp, Warning, TEXT("CurrentStressAmount <= 0.f , Can not Use Item"));
				return;
			}
			if (Consume->bPeriodicStressIncreaseCancle)
			{
				StressSS->ClearStressTimer();
			}
			UE_LOG(LogTemp, Warning, TEXT("Consume Decrease %f"), Consume->ImmediateStessDecrease);
			StressSS->ChangeStressAmount(Consume->ImmediateStessDecrease);
			
		}
	}

	if (GameFlowManager && Consume->UseEventTag.IsValid())
	{
		GameFlowManager->NotifyObjectiveCompleted(Consume->UseEventTag);
	}

	// 사용 후 슬롯 비우기 + UI 반영
	UnRegisterItem(Index);
	
	//Test 끝
}

TSoftObjectPtr<UTexture2D> USRQuickSlotComponent::ResolveIconByItemId(FName Id) const
{

	return TSoftObjectPtr<UTexture2D>();
}

