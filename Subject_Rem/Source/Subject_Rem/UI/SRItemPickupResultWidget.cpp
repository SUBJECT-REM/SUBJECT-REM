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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASRItemPreview::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ASRItemPreview* PreviewActor = Cast<ASRItemPreview>(FoundActors[0]);
		if (PreviewActor)
		{
			PreviewActor->ReplaceStaticMesh(Mesh.LoadSynchronous());
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASRItemPreview not found"));
	}
}

void USRItemPickupResultWidget::SetItemDes(FName Text)
{
	ItemDes->SetText(FText::FromName(Text));
}

void USRItemPickupResultWidget::SetItemName(FName Text)
{
	ItemName->SetText(FText::FromName(Text));
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
			break;
		}

		default:
			break;
		};
	}
}
