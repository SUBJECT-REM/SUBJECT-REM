// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRCluemapCombinedDesWidget.h"
#include"Components/Image.h"
#include "Components/RichTextBlock.h"
#include"Components/Button.h"
#include "Components/WidgetSwitcher.h"


void USRCluemapCombinedDesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseButtonClicked);
}

void USRCluemapCombinedDesWidget::SetClueIcons(TArray<TSoftObjectPtr<UTexture2D>> Icons)
{
	const int32 N = Icons.Num();
    if (LayoutSwitcher)
    {
        LayoutSwitcher->SetActiveWidgetIndex(N == 3 ? 1 : 0); // ← 세미콜론 빠져있었음!
    }

    if (N == 2)
    {
        // .h의 이름과 일치하도록 사용(ClueLeftImage/ClueRightImage)
        SetImageBrush(ClueLeftImage, Icons[0]);
        SetImageBrush(ClueRightImage, Icons[1]);
    }
    else if (N == 3)
    {
        SetImageBrush(ClueLeftImage_3, Icons[0]);
        SetImageBrush(ClueMidImage_3, Icons[1]);
        SetImageBrush(ClueRightImage_3, Icons[2]);
    }
}

void USRCluemapCombinedDesWidget::SetClueNamesText(const TArray<FText>& Text)
{
    const int32 N = Text.Num();
    EnsureLayoutByCount(N);

    if (N == 3)
    {
        SetRichText(LeftImageItemName_3, Text[0]);
        SetRichText(MidImageItemName_3, Text[1]);
        SetRichText(RightImageItemName_3, Text[2]);

        HideRichText(LeftImageItemName);
        HideRichText(RightImageItemName);
    }
    else // N != 3 → 2로 처리
    {
        SetRichText(LeftImageItemName, Text.IsValidIndex(0) ? Text[0] : FText::GetEmpty());
        SetRichText(RightImageItemName, Text.IsValidIndex(1) ? Text[1] : FText::GetEmpty());

        HideRichText(LeftImageItemName_3);
        HideRichText(MidImageItemName_3);
        HideRichText(RightImageItemName_3);
    }
}

void USRCluemapCombinedDesWidget::SetClueMapName(FText Name)
{
	ClueMapName->SetText(Name);
}

void USRCluemapCombinedDesWidget::SetClueMapDes(FText Des)
{
	ClueMapDes->SetText(Des);
}

void USRCluemapCombinedDesWidget::OnCloseButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USRCluemapCombinedDesWidget::SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D>& SoftTex)
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

    else
    {
        Image->SetBrushFromSoftTexture(SoftTex);
        Image->SetVisibility(ESlateVisibility::Visible);
    }
}

void USRCluemapCombinedDesWidget::SetRichText(URichTextBlock* Label, const FText& Text)
{
    if (!Label) return;
    Label->SetText(Text);
    Label->SetVisibility(ESlateVisibility::Visible);
}

void USRCluemapCombinedDesWidget::HideRichText(URichTextBlock* Label)
{
    if (!Label) 
        return;
    Label->SetText(FText::GetEmpty());
    Label->SetVisibility(ESlateVisibility::Collapsed);
}

void USRCluemapCombinedDesWidget::EnsureLayoutByCount(uint8 Count)
{
    // 2/3 외 값 방어
    const int32 Cnt = (Count == 3) ? 3 : 2;
    if (LayoutSwitcher)
    {
        LayoutSwitcher->SetActiveWidgetIndex(Cnt == 3 ? 1 : 0);
    }
}
