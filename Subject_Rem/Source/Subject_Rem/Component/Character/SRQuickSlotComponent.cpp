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
	return (SlotIcons.IsValidIndex(Index)) ? Slots[Index] : NAME_None;
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

	UnRegisterItem(QuickSlotNum - 1);
	//Test - 필요하면 아래 if - else 문 삭제해도 됩니다.
	if (QuickSlotNum == 1)
	{
		/*if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				StressSubsystem->ChangeStressAmount(1.0f);
			}
		}*/


	}
	else if (QuickSlotNum == 2)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Quick 2"));
				StressSubsystem->ChangeStressAmount(-1.0f);
			}
		}
	}
	else if (QuickSlotNum == 3)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Quick 3"));
				StressSubsystem->ChangeStressByTime(5.0f, 1.0f);
			}
		}
	}
	//Test 끝
}

TSoftObjectPtr<UTexture2D> USRQuickSlotComponent::ResolveIconByItemId(FName Id) const
{

	return TSoftObjectPtr<UTexture2D>();
}

