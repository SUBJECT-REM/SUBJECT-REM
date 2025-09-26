// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRItemPickupResultWidget.h"
#include "UI/SRRotateItemPreviewWidget.h"
#include "Actor/SmoothRotateACtor/SRItemPreview.h"
#include "Component/SRRotateableStaticMeshComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"


void USRItemPickupResultWidget::NativeConstruct()
{
	OnVisibilityChanged.AddDynamic(this, &ThisClass::HandleVisibilityChange);
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
	}

	ItemPreview->SetWidth(ItemPreviewWidgetWidth);
	ItemPreview->SetHeight(ItemPreviewWidgetHeight);
}

void USRItemPickupResultWidget::SetItemPreview(TSoftObjectPtr<UStaticMesh> Mesh)
{
	ItemPreview->SetItemPreviewMesh(Mesh);
}

void USRItemPickupResultWidget::SetItemDes(FText Text)
{
	ItemDes->SetText(Text);
	ItemDes->InvalidateLayoutAndVolatility();
	ItemDes->ForceLayoutPrepass();
}

void USRItemPickupResultWidget::SetItemName(FText Text)
{
	ItemName->SetText(Text);
	ItemName->InvalidateLayoutAndVolatility();
	ItemName->ForceLayoutPrepass();
}

FReply USRItemPickupResultWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::SpaceBar)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);

}

void USRItemPickupResultWidget::HandleVisibilityChange(ESlateVisibility NewVisibility)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		switch (NewVisibility)
		{
		case ESlateVisibility::Visible:
		{	// 최초 생성 시에도 입력 모드 설정
			FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TakeWidget());
				PC->SetInputMode(InputMode);
				break;
		}
		case ESlateVisibility::Hidden:
		case ESlateVisibility::Collapsed:
		{
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
			OnClosedDelegate.Broadcast();
			break;
		}

		default:
			break;
		};
	}
}
