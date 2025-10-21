// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter/SRItemPickupResultPresenter.h"
#include "Component/SRInventoryComponent.h"
#include "UI/SRItemPickupResultWidget.h"
#include "SRItemData.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Manager/SRCaptionManagerActor.h"

void USRItemPickupResultPresenter::Init(UActorComponent* InitComponent, UUserWidget* InitWidget)
{
	Super::Init(InitComponent, InitWidget);

	InvenComp = Cast<USRInventoryComponent>(InitComponent);
	if (!InvenComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitComponent Cast cannot be cast to InventoryComponent"));
		return;
	}
	check(InvenComp);

	//InvenComp->AddInventoryDataDelegate.AddDynamic(this, &ThisClass::ShowItemPickWidget);
	InvenComp->ItemPickupDelegate.AddDynamic(this, &ThisClass::ShowItemPickWidget);
}

void USRItemPickupResultPresenter::ShowItemPickWidget(const FSRItemBaseData& ShownItemData)
{
	//TODO 위젯생성은 HUD에 위임하기 
	if (!ItemPickupResultWidget)
	{
		//ItemPickupResultWidget에서는 Space입력시 닫아야함. 여기서 필요시 PlayerController를 넘겨줘야할거같음.
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ItemPickupResultWidget = CreateWidget<USRItemPickupResultWidget>(PC, ItemPickUpResultWidgetClass);
			// Presenter에서 바인딩할 때
			if (!ItemPickupResultWidget->OnClosedDelegate.IsAlreadyBound(this, &ThisClass::HandleWidgetClose))
			{
				ItemPickupResultWidget->OnClosedDelegate.AddDynamic(this, &ThisClass::HandleWidgetClose);
				
				ChashedCaptionManager = Cast<ASRCaptionManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASRCaptionManagerActor::StaticClass()));
			}
		}
		check(ItemPickupResultWidget);
	}
	CashedData = ShownItemData;
	CashedCaptionDataRow = ShownItemData.PickupCaptionRow;
	if (ChashedCaptionManager.IsValid())
	{
		ChashedCaptionManager->NotifyPickupResultToggle(true);
	}
	// 2) 픽업 캡션 미리 큐에 넣어둠 (닫히면 재생됨)
	if (!CashedCaptionDataRow.RowName.IsNone())
	{
		ChashedCaptionManager->PlayCaptionImmediateNext(CashedCaptionDataRow.RowName,false);
	}

	ItemPickupResultWidget->SetItemPreview(ShownItemData.Mesh);
	ItemPickupResultWidget->SetItemName(ShownItemData.Name);
	ItemPickupResultWidget->SetItemDes(ShownItemData.PickupDescription);
	UE_LOG(LogTemp, Warning, TEXT("PickupResultWidget Item Name  : %s"), *ShownItemData.Name.ToString());
	if (!ItemPickupResultWidget->IsInViewport())
	{
		ItemPickupResultWidget->AddToViewport(10);
	}

	if (ItemPickupResultWidget->GetVisibility() == ESlateVisibility::Hidden || ItemPickupResultWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		ItemPickupResultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USRItemPickupResultPresenter::HandleWidgetVisibilityChanged(ESlateVisibility NewVisibility)
{
	if (NewVisibility == ESlateVisibility::Visible)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
}

void USRItemPickupResultPresenter::HandleWidgetClose()
{
	//ChashedCaptionManager->EnqueueCaption(CashedCaptionDataRow.RowName);
	if (ChashedCaptionManager.IsValid())
	{
		ChashedCaptionManager->NotifyPickupResultToggle(false); // 닫히는 순간 재생 재개
	}
	if (CashedData.PickupResultCloseSfx)
	{
		// 페이드 타이밍을 맞추고 싶으면 약간 지연도 OK:
		// GetWorld()->GetTimerManager().SetTimerForNextTick([this, Data]{
		//     UGameplayStatics::PlaySound2D(GetWorld(), Data->PickupResultCloseSfx);
		// });
		UGameplayStatics::PlaySound2D(GetWorld(), CashedData.PickupResultCloseSfx);
	}
}



