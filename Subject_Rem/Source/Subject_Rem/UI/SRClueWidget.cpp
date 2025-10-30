// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueWidget.h"
#include "UI/SRSlotWidget.h"
#include "UI/SRClueCombineResultWidget.h"
#include "UI/SRClueCombineWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridPanel.h"

#include "Kismet/GameplayStatics.h"



void USRClueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ClueGridPanel)
	TArray<UWidget*> ClueGridChild = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : ClueGridChild)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(ClueSlot);

		ClueSlot->Clear();
		ClueSlot->OnSlotDropedDelegate.AddDynamic(this, &ThisClass::OnSlotDropped_Clue);
		ClueSlot->SetSlotButtonNormalStyle(ClueSlotButtonNormalStyle);
		ClueSlot->SetSlotButtonSelectedStyle(ClueSlotButtonSelectedStyle);
	}
	
	ClueCombineWidget->OnCombineSlotDropped.AddUniqueDynamic(this, &ThisClass::OnSlotDropped_ClueCombine);
	ClueCombineWidget->SetLayoutByCount(1);
	check(DeviceGridPanel)
		TArray<UWidget*> DeviceGridChild = DeviceGridPanel->GetAllChildren();

	for (UWidget* Widget : DeviceGridChild)
	{
		USRSlotWidget* DeviceSlot = Cast<USRSlotWidget>(Widget);
		if (!DeviceSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClueCombineGridPanel Children Cast cannot be cast to USRSlotWidget"));
		}
		check(DeviceSlot);
		DeviceSlot->OnSlotClickedDelegate.AddDynamic(this, &ThisClass::OnClickedDeviceSlot);
		DeviceSlot->Clear();
	}
	check(ClueCombineButton)

	ClueCombineButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCombineButton);
	DevicePutBackButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedDevicePutBackButton);

	SetVisibility(ESlateVisibility::Hidden);
}

void USRClueWidget::UpdateClueGridWidget(const FSRItemBaseData& Data)
{
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		check(ClueSlot);

		if (!ClueSlot->GetIsOccupied())
		{
			ClueSlot->Fill(Data);
			break;
		}
	}
}

void USRClueWidget::UpdateClueCombineResultWidget(const FSRClueCombineResultUIData& Data)
{
	
	ClueCombineResultWidget->SetClueMapImage(Data.ClueIcons);
	ClueCombineResultWidget->SetClueMapDes(Data.ClueMap.Description);
	ClueCombineResultWidget->SetClueMapName(Data.ClueMap.Name);
	if (ClueCombineResultWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		ClueCombineResultWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void USRClueWidget::UpdateDeviceGridWidget(const FSRDeviceUIData& Data)
{
	TArray<UWidget*> Child = DeviceGridPanel->GetAllChildren();
	for (UWidget* Widget : Child)
	{
		USRSlotWidget* DeviceSlot = Cast<USRSlotWidget>(Widget);
		check(DeviceSlot);

		if (!DeviceSlot->GetIsOccupied())
		{
			DeviceSlot->Fill(Data.Base);
			CashedDeviceUsingClueNum.Add(Data.Base.Id, Data.UsingSlotNum);
			CachedDeviceAllowedMap.Add(Data.Base.Id, Data.AllowedItemIds);

			break;
		}
	}
}


bool USRClueWidget::CanAccepDropClueGridPanel(const FSRItemBaseData& Item, USRSlotWidget* From, USRSlotWidget* To)
{
	if (!From || !To) return false;

	// 대상 패널
	const bool bToIsCombine = (To->GetParent() == GetCurrentClueCombineGrid());

	// 이미 찬 슬롯 금지
	if (To->GetIsOccupied()) return false;

	// 조합칸 슬롯 개수 제한
	if (bToIsCombine)
	{
		int32 Occupied = 0;
		for (UWidget* W : GetCurrentClueCombineGrid()->GetAllChildren())
			if (const USRSlotWidget* S = Cast<USRSlotWidget>(W))
				if (S->GetIsOccupied()) ++Occupied;

		if (Occupied >= CurVaildCombineItemNum)
			return false;
	}

	// 디바이스 상태
	const bool bDeviceOpen = (CurUsingDevicedSlot != nullptr);
	const FName ActiveDeviceId = bDeviceOpen ? CurUsingDevicedSlot->GetItemData().Id : NAME_None;

	// [규칙 A] 이 아이템이 디바이스 필수인데, 디바이스가 안 열려있으면 금지
	if (bToIsCombine && Item.RequiredDeviceId != NAME_None && !bDeviceOpen)
		return false;

	// [규칙 B] 디바이스가 열려있는데, 이 아이템이 요구하는 디바이스와 다르면 금지
	if (bToIsCombine && bDeviceOpen && Item.RequiredDeviceId != NAME_None &&
		Item.RequiredDeviceId != ActiveDeviceId)
		return false;

	// [규칙 C] 디바이스가 열려있고, 디바이스가 허용 목록만 받도록 되어 있다면 목록 체크
	if (bToIsCombine && bDeviceOpen)
	{
		const TArray<FName>* Allowed = CachedDeviceAllowedMap.Find(ActiveDeviceId);
		if (!Allowed || !Allowed->Contains(Item.Id))
			return false;
	}

	return true;
}

bool USRClueWidget::ValidateClueCombineDrop(const FSRItemBaseData& Item, USRSlotWidget* From, USRSlotWidget* To)
{
	// 3) 디바이스 상태
	const bool bDeviceOpen = (CurUsingDevicedSlot != nullptr);
	const FName ActiveDeviceId = bDeviceOpen ? CurUsingDevicedSlot->GetItemData().Id : NAME_None;

	// 4) 아이템이 특정 디바이스를 **필수**로 요구하면, 그 디바이스가 열려 있어야 함
	if (Item.RequiredDeviceId != NAME_None && !bDeviceOpen)
	{
		return false;
	}
	// 5) 열려 있는 디바이스와 요구 디바이스 불일치 금지
	if (bDeviceOpen && Item.RequiredDeviceId != NAME_None && Item.RequiredDeviceId != ActiveDeviceId)
	{
		return false;
	}
	// 6) (선택) 허용 목록 강제 모드면, 목록에 없는 아이템 금지
	if (bDeviceOpen)
	{
		const TArray<FName>* Allowed = CachedDeviceAllowedMap.Find(ActiveDeviceId);
		if (!Allowed)
		{
			return false;
		}
		if (!Allowed->Contains(Item.Id))
		{
			return false;
		}
		
	}

	return true;
}

void USRClueWidget::RevertDrop(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot, const FSRItemBaseData& MovedData)
{
	if (!DroppedSlot || !DraggedSlot) return;

	// 목적지 비우기
	DroppedSlot->Clear();
	// 원래 슬롯 복구
	DraggedSlot->Fill(MovedData);


}

void USRClueWidget::DeactivateCurrentDevice()
{
	//Default상태에서 다른 다비아스 클릭시에 대한 처리
	if (!CurUsingDevicedSlot)
	{
		UPanelWidget* ActiveCombine = GetCurrentClueCombineGrid();
		MoveAllFromCombineToClue(ActiveCombine);
	}

	// 현재 활성 탭(조합 그리드)에서 아이템을 전부 회수
	UPanelWidget* ActiveCombine = GetCurrentClueCombineGrid();
	MoveAllFromCombineToClue(ActiveCombine);

	// 디폴트(2칸)로 전환 & 상태 초기화
	//ClueCombineSwitcher->SetActiveWidgetIndex(1); 
	ClueCombineWidget->SetLayoutByCount(1);
	CurVaildCombineItemNum = 2;
	CurUsingDevicedSlot = nullptr;

}

void USRClueWidget::MoveAllFromCombineToClue(UPanelWidget* FromGrid)
{
	if (!FromGrid) return;

	// 1) 현재 조합칸에서 데이터 수집(순서 유지)
	TArray<FSRItemBaseData> ToMove;
	for (UWidget* W : FromGrid->GetAllChildren())
	{
		if (auto* S = Cast<USRSlotWidget>(W))
		{
			if (S->GetIsOccupied())
			{
				ToMove.Add(S->GetItemData());
				S->Clear(); // 비우기
			}
		}
	}

	// 2) 클루 그리드의 앞쪽 빈 칸부터 채우기
	for (const FSRItemBaseData& Data : ToMove)
	{
		if (auto* Dest = FindFirstEmptyClueSlot())
		{
			Dest->Fill(Data);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No free ClueGrid slot to return item %s"),
				   *Data.Id.ToString());
			// 필요 시: 남은 아이템을 버퍼에 보관하거나, 사용자 알림 처리
		}
	}
}

USRSlotWidget* USRClueWidget::FindFirstEmptyClueSlot() const
{
	for (UWidget* W : ClueGridPanel->GetAllChildren())
		if (auto* S = Cast<USRSlotWidget>(W))
			if (!S->GetIsOccupied())
				return S;
	return nullptr;
}

void USRClueWidget::OnSlotDropped_Clue(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot)
{
	if (!DroppedSlot || !DraggedSlot) return;

	const FSRItemBaseData DroppedData = DroppedSlot->GetItemData(); // 지금 목적지에 들어간 데이터

	if (!DroppedData.OptionalResouce.IsNull())
	{
		ClueCombineWidget->HandleOptionalResource(DraggedSlot, nullptr);
	}
}

void USRClueWidget::OnSlotDropped_ClueCombine(USRSlotWidget* DroppedSlot, USRSlotWidget* DraggedSlot)
{
	if (!DroppedSlot || !DraggedSlot) return;

	const FSRItemBaseData DroppedData = DroppedSlot->GetItemData(); // 지금 목적지에 들어간 데이터
	UE_LOG(LogTemp, Warning, TEXT("OnSlotDropped_ClueCombine"));
	// 1) 조건 검사
	if (!ValidateClueCombineDrop(DroppedData, DraggedSlot, DroppedSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSlotDropped_ClueCombine :: VaildateClueCombineDrop False"));
		// 2) 실패 → 리버트(원상복구)
		RevertDrop(DroppedSlot, DraggedSlot, DroppedData);
		return;
	}

	//DroppedData.OptionalResouces

	UE_LOG(LogTemp, Warning, TEXT("OnSlotDropped_ClueCombine :: VaildateClueCombineDrop Success"));
	if (!DroppedData.OptionalResouce.IsNull())
	{
		ClueCombineWidget->HandleOptionalResource(DraggedSlot, nullptr);
		ClueCombineWidget->HandleOptionalResource(DroppedSlot, DroppedData.OptionalResouce);
	}

}

void USRClueWidget::ClueDataMoveToClueCombine(USRSlotWidget* ClickedSlot)
{
	TArray<UWidget*> Child = GetCurrentClueCombineGrid()->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		
		USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);
		if (!ClueSlot->GetIsOccupied())
		{
			ClueSlot->Fill(ClickedSlot->GetItemData());
			ClickedSlot->Clear();
			ClickedSlot->SetIsEnabled(true);
			break;
		}			
	}
	
}

void USRClueWidget::ClueCombineDataMoveToClue(USRSlotWidget* ClickedSlot)
{
	//check(ClueGridPanel)
	
	TArray<UWidget*> Child = ClueGridPanel->GetAllChildren();

	for (UWidget* Widget : Child)
	{
		if (!Widget->GetIsEnabled())
		{
			USRSlotWidget* ClueSlot = Cast<USRSlotWidget>(Widget);

			ClueSlot->Fill(ClickedSlot->GetItemData());
			ClickedSlot->Clear();
			ClickedSlot->SetIsEnabled(true);

			break;
		}

	}
	
}

void USRClueWidget::OnClickedCombineButton()
{
	//check(ClueCombineGridPanel)

	TArray<FName> CombinedClueIds;

	for (UWidget* Widget : GetCurrentClueCombineGrid()->GetAllChildren())
	{
		USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);
		FSRItemBaseData Data = ClueCombineSlot->GetItemData();
		if (!Data.Id.IsNone())
		{
			CombinedClueIds.Add(Data.Id);
		}
	}

	if (CombinedClueIds.Num() == CurVaildCombineItemNum)
	{
		CombineButtonClickedDelegate.Broadcast(CombinedClueIds);
		
		for (UWidget* Widget : GetCurrentClueCombineGrid()->GetAllChildren())
		{
			USRSlotWidget* ClueCombineSlot = Cast<USRSlotWidget>(Widget);

			ClueCombineWidget->HandleOptionalResource(ClueCombineSlot, nullptr);
			//조합 이후 ClueCombinePanel 아이템들 제거
			ClueCombineSlot->Clear();
			ClueCombineSlot->SetIsEnabled(true);
		}
	}



}

void USRClueWidget::OnClickedDevicePutBackButton()
{
	ClueCombineWidget->PutBackDevice();
	DeactivateCurrentDevice();

	if (GetWorld()->GetTimerManager().IsTimerActive(AudioPickupSoundTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AudioPickupSoundTimer);
	}
}

void USRClueWidget::OnClickedDeviceSlot(USRSlotWidget* ClickedSlot)
{
	if (!ClickedSlot) return;

	const FName DeviceId = ClickedSlot->GetItemData().Id;
	if (DeviceId.IsNone()) return;

	DeactivateCurrentDevice();

	// 새 디바이스 활성화
	uint8* NumPtr = CashedDeviceUsingClueNum.Find(DeviceId);
	if (!NumPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnClickedDeviceSlot: UsingClueNum not found for %s"), *DeviceId.ToString());
		return;
	}

	CurVaildCombineItemNum = *NumPtr;
	ClueCombineWidget->SetLayoutByCount(CurVaildCombineItemNum - 1);
	CurUsingDevicedSlot = ClickedSlot;

	if (CurVaildCombineItemNum - 1 == 2) // audio 
	{
		GetWorld()->GetTimerManager().SetTimer(
		AudioPickupSoundTimer,
		[this]()
		{
		if (UWorld* InWorld = GetWorld())
		{
			if (PlayedSound)
			{
				UGameplayStatics::PlaySound2D(InWorld, PlayedSound);
			}
		}
		},
		8.f,   // Rate
		true,       // bLooping
		0.f    // FirstDelay (5초 뒤부터 반복 시작)
			);
	}

}

UPanelWidget* USRClueWidget::GetCurrentClueCombineGrid()
{
	return ClueCombineWidget->GetActivePanelWidget();
}
