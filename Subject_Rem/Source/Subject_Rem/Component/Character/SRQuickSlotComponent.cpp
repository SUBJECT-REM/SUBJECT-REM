// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/SRQuickSlotComponent.h"
#include "Subsystem/SRStressLocalPlayerSubsystem.h"
#include "SRItemData.h"
// Sets default values for this component's properties
USRQuickSlotComponent::USRQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Slots.Init(NAME_None, 3);
	SlotIcons.Init(nullptr, 3); 
}

// Called when the game starts
void USRQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (int i = 0; i < StartQuickItemDatas.Num(); i++)
	{
		RegisterItem(i, StartQuickItemDatas[i].RowName);
	}
	
}

void USRQuickSlotComponent::PressQuickSlot(uint8 QuickSlotNum)
{
	UseQuickSlotItem(QuickSlotNum);
}

void USRQuickSlotComponent::RegisterItem(uint8 Index, FName Id)
{
	if (!Slots.IsValidIndex(Index) || Id.IsNone() || !ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slots Index or Id is not vaild : QuickSlotComp, RegisterItem"));
		return;
	}

	FString FindQuickItemContext;
	FSRItemData* FindData = ItemDataTable->FindRow<FSRItemData>(Id, FindQuickItemContext);
	
	if (!FindData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable FindRow is not vaild : QuickSlotComp, RegisterItem"));
		return;
	}
	
	//SubDT 가져오기
	const FDataTableRowHandle& Handle = FindData->ItemDataTable;
	if (Handle.IsNull() || !Handle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindRow - FindData->ItemDataTable  is not vaild : QuickSlotComp, RegisterItem"));
		return;
	}

	//SubDT가 ConsumeData가 아니라면 return;
	const UDataTable* SubDT = Handle.DataTable;
	if (SubDT->GetRowStruct() != FSRConsumeData::StaticStruct())
	{

		UE_LOG(LogTemp, Warning, TEXT("FindRow DataTable is not SRConsumeData : QuickSlotComp, RegisterItem"));
		return;
	}

	const FSRConsumeData* ConsumeRow = SubDT->FindRow<FSRConsumeData>(Handle.RowName, FindQuickItemContext);
	if (!ConsumeRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConsumeDatTable FindRow is not vaild : QuickSlotComp, RegisterItem"));
		return;
	}

	Slots[Index] = Id;
	SlotIcons[Index] = FindData->BaseInfo.Icon; 
	OnQuickSlotChangedDelegate.Broadcast(Index, FindData->BaseInfo.Icon);
}

void USRQuickSlotComponent::UnRegisterItem(uint8 Index)
{
	if (!Slots.IsValidIndex(Index))
		return;

	Slots[Index] = NAME_None;
	SlotIcons[Index] = nullptr;

	OnQuickSlotChangedDelegate.Broadcast(Index, nullptr);
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
\
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
	////Test
	if (QuickSlotNum == 2)
	{
		//즉시 감소
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Quick 2"));
				StressSubsystem->ClearStressTimer();
				StressSubsystem->ChangeStressAmount(-20.0f);

			}
		}
	}
	else if (QuickSlotNum == 3)
	{
		//즉시 증가.
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Quick 3"));
				StressSubsystem->ChangeStressAmount(+20.f);
			}
		}
	}
	else if (QuickSlotNum == 1)
	{
		//일정 시간 증가.
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Quick 1"));
				StressSubsystem->ChangeStressByTime(5.f,5.f);
			}
		}
	}


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
			if (Consume->bPeriodicStressIncreaseCancle)
			{
				StressSS->ClearStressTimer();
			}
			UE_LOG(LogTemp, Warning, TEXT("Consume Decrease %f"), Consume->ImmediateStessDecrease);
			StressSS->ChangeStressAmount(Consume->ImmediateStessDecrease);
			

		}
	}


	// 사용 후 슬롯 비우기 + UI 반영
	UnRegisterItem(Index);
	
	//Test 끝
}

TSoftObjectPtr<UTexture2D> USRQuickSlotComponent::ResolveIconByItemId(FName Id) const
{

	return TSoftObjectPtr<UTexture2D>();
}

