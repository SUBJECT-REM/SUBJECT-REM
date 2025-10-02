// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueCombineResultWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Components/WidgetSwitcher.h"

void USRClueCombineResultWidget::SetClueMapImage(TArray<TSoftObjectPtr<UTexture2D>> Icons)
{
	const int32 N = Icons.Num();

	EnsureLayoutByCount(Icons.Num());
	if (N == 2)
	{
			SetImageBrush(ClueLeftImage, Icons[0]);
			SetImageBrush(ClueRightImage, Icons[1]);
	}
	else if (N == 3)
	{
		SetImageBrush(ClueLeftImage_3, Icons[0]);
		SetImageBrush(ClueMidImage_3, Icons[1]);
		SetImageBrush(ClueRightImage_3, Icons[2]);
	}
	else
	{
		SetImageBrush(ClueImage, Icons[0]);
	}
}

void USRClueCombineResultWidget::SetClueMapDes(FText Text)
{
	ClueMapDes->SetText(Text);
}

void USRClueCombineResultWidget::SetClueMapName(FText Text)
{
	ClueMapName->SetText(Text);
}

void USRClueCombineResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ClueLeftImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	ClueRightImage->SetVisibility(ESlateVisibility::HitTestInvisible);;
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseButton);
}

void USRClueCombineResultWidget::OnClickedCloseButton()
{
	UE_LOG(LogTemp, Warning, TEXT("USRClueCombineResultWidget :: OnClicked Close Buton"));
	SetVisibility(ESlateVisibility::Collapsed);
}

void USRClueCombineResultWidget::EnsureLayoutByCount(uint8 Count)
{
	const int32 Index = Count - 1;
	if (LayoutSwitcher)
	{
		LayoutSwitcher->SetActiveWidgetIndex(Index);
	}
}

void USRClueCombineResultWidget::SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D>& SoftTex)
{
	if (!Image)
		return;

	// 소프트 레퍼런스가 비었으면 감추기
	if (SoftTex.IsNull())
	{
		Image->SetBrushFromTexture(nullptr);
		Image->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 동기 로드 (프로토타입/에디터라면 OK, 추후 비동기 전환 가능)
	if (!SoftTex.IsNull())
	{
		Image->SetBrushFromSoftTexture(SoftTex);
		Image->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 로드 실패 시 안전 처리
		Image->SetBrushFromSoftTexture(nullptr);
		Image->SetVisibility(ESlateVisibility::Collapsed);
	}
}
