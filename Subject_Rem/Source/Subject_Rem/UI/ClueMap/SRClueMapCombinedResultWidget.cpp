// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void USRClueMapCombinedResultWidget::NativeConstruct()
{
     
      SetVisibility(ESlateVisibility::Visible);
     
      ClueMapDesShownButton->OnClicked.AddDynamic(this, &ThisClass::OnCobminedClueButtonClicked);
      LeftClueImage->SetVisibility(ESlateVisibility::Hidden);
      RightClueImage->SetVisibility(ESlateVisibility::Hidden);
}

void USRClueMapCombinedResultWidget::UpdateClueImage(TArray<TSoftObjectPtr<UTexture2D>> Icons)
{
    // 2/3 외 값 방어
    const int32 Cnt = (Icons.Num() == 3) ? 3 : 2;
    if (LayoutSwitcher)
    {
        LayoutSwitcher->SetActiveWidgetIndex(Cnt == 3 ? 1 : 0);
    }

    if (Cnt == 2)
    {
        // .h의 이름과 일치하도록 사용(ClueLeftImage/ClueRightImage)
        SetImageBrush(LeftClueImage, Icons[0]);
        SetImageBrush(RightClueImage, Icons[1]);
    }
    else if (Cnt == 3)
    {
        SetImageBrush(LeftClueImage_3, Icons[0]);
        SetImageBrush(MidImage_3, Icons[1]);
        SetImageBrush(RightClueImage_3, Icons[2]);
    }
}

void USRClueMapCombinedResultWidget::OnCobminedClueButtonClicked()
{
   
    if(!CombinedClueID.IsNone())
    {
        ClueMapCombinedResultClickedDelegate.Broadcast(CombinedClueID);
    }

    //델리게이트로 넘김 
}

void USRClueMapCombinedResultWidget::SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D>& SoftTex)
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
