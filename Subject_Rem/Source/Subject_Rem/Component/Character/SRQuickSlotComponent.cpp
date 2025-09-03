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
}

// Called when the game starts
void USRQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USRQuickSlotComponent::PressQuickSlot(uint8 QuickSlotNum)
{
	UseQuickSlotItem(QuickSlotNum);
}

void USRQuickSlotComponent::RegisterItem(uint8 Index, FName Id)
{
	if (!Slots.IsValidIndex(Index) || Id.IsNone() || !ItemDataTable)
	{
		return;
	}

	FString FindQuickItemContext;
	FSRItemData* FindData = ItemDataTable->FindRow<FSRItemData>(Id, FindQuickItemContext);
	
	if (!FindData)
	{
		return;
	}
	
	//SubDT 가져오기
	const FDataTableRowHandle& Handle = FindData->ItemDataTable;
	if (Handle.IsNull() || !Handle.DataTable)
	{
		return;
	}

	//SubDT가 ConsumeData가 아니라면 return;
	const UDataTable* SubDT = Handle.DataTable;
	if (SubDT->GetRowStruct() != FSRConsumeData::StaticStruct())
	{
		return;
	}

	const FSRConsumeData* ConsumeRow = SubDT->FindRow<FSRConsumeData>(Handle.RowName, FindQuickItemContext);
	if (!ConsumeRow)
	{
		return;
	}

	Slots[Index] = Id;
	OnQuickSlotChangedDelegate.Broadcast(Index, FindData->BaseInfo.Icon);
}

void USRQuickSlotComponent::UnRegisterItem(uint8 Index)
{
	if (!Slots.IsValidIndex(Index))
		return;

	Slots[Index] = NAME_None;

	OnQuickSlotChangedDelegate.Broadcast(Index, nullptr);
}

FName USRQuickSlotComponent::GetItemIdBySlotIndex(uint8 Index)
{
	if (!Slots[Index].IsNone())
		return Slots[Index];

	return NAME_None;
}


void USRQuickSlotComponent::UseQuickSlotItem(uint8 QuickSlotNum)
{
	UE_LOG(LogTemp, Log, TEXT("UseQuickSlot : %d"), QuickSlotNum);

	//Test - 필요하면 아래 if - else 문 삭제해도 됩니다.
	if (QuickSlotNum == 1)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
				StressSubsystem->ChangeStressAmount(1.0f);
			}
		}
	}
	else if (QuickSlotNum == 2)
	{
		if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (USRStressLocalPlayerSubsystem* StressSubsystem = LocalPlayer->GetSubsystem<USRStressLocalPlayerSubsystem>())
			{
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
				StressSubsystem->ChangeStressByTime(5.0f, 1.0f);
			}
		}
	}
	//Test 끝
}


// Called every frame
void USRQuickSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//생성자에서 비활성화했습니다.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

