// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void USRClueMapCombinedResultWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
}

void USRClueMapCombinedResultWidget::NativeConstruct()
{
    Super::NativeConstruct();
      SetVisibility(ESlateVisibility::Visible);
     
      ClueMapDesShownButton->OnClicked.AddDynamic(this, &ThisClass::OnCobminedClueButtonClicked);
      LeftClueImage->SetVisibility(ESlateVisibility::Collapsed);
      RightClueImage->SetVisibility(ESlateVisibility::Collapsed);

      LeftClueImage_3->SetVisibility(ESlateVisibility::Collapsed);
      MidImage_3->SetVisibility(ESlateVisibility::Collapsed);
      RightClueImage_3->SetVisibility(ESlateVisibility::Collapsed);
}

void USRClueMapCombinedResultWidget::UpdateClueImage(TArray<TSoftObjectPtr<UTexture2D>> Icons)
{
    // 2/3 외 값 방어
   // const int32 Cnt = (Icons.Num() == 3) ? 3 : 2;

    UE_LOG(LogTemp, Warning, TEXT("UpdateClueImage Icons Num :%d"), Icons.Num());
    if (LayoutSwitcher)
    {
        LayoutSwitcher->SetActiveWidgetIndex(Icons.Num() == 3 ? 1 : 0);
    }

    if (Icons.Num() == 2)
    {
        // .h의 이름과 일치하도록 사용(ClueLeftImage/ClueRightImage)
        SetImageBrush(LeftClueImage, Icons[0]);
        SetImageBrush(RightClueImage, Icons[1]);
    }
    else if (Icons.Num() == 3)
    {
        SetImageBrush(LeftClueImage_3, Icons[0]);
        SetImageBrush(MidImage_3, Icons[1]);
        SetImageBrush(RightClueImage_3, Icons[2]);

        ClueMapDesShownButton->SetStyle(ButtonThreeStyle);
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

void USRClueMapCombinedResultWidget::SetImageBrush(UImage* Image, const TSoftObjectPtr<UTexture2D> SoftTex)
{
    if (!Image)
        return;

    // 소프트 레퍼런스가 비었으면 감추기
    if (SoftTex.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("SetImageBrush SoftText Null"));
        Image->SetBrushFromSoftTexture(nullptr);
        Image->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetImageBrush SoftText"));

        Image->SetBrushFromSoftTexture(SoftTex);
        Image->SetVisibility(ESlateVisibility::Visible);
    }
}
