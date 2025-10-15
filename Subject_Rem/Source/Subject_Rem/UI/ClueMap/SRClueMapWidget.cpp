// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapWidget.h"
#include "UI/ClueMap/SRCluemapCombinedDesWidget.h"
#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "UI/ClueMap/SRTrueClueLinkWidget.h"

#include "Components/CanvasPanel.h"
#include "Component/SRInventoryComponent.h"
#include"Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Components/CanvasPanelSlot.h"

void USRClueMapWidget::NativeOnInitialized()
{
      UE_LOG(LogTemp, Warning, TEXT("USRClueMapWidget NativeOnInit"));
      
      FindPlayerInventoryComponent();
      BindInventoryDelegate();

      if (!InventoryComponent)
      {
            UE_LOG(LogTemp, Warning, TEXT("Not Find Inven"));
      }

      FindCombinedResultWidgets();
      FindTrueClueLinkWidgets();
}

void USRClueMapWidget::NativeConstruct()
{
      Super::NativeConstruct();
      //UE_LOG(LogTemp, Warning, TEXT("NativeConstruct - ClueMap"));
      
}

void USRClueMapWidget::NativeDestruct()
{
      Super::NativeDestruct();
      const int32 ChildCount = RootCanvasPanel->GetChildrenCount();

      for (int32 i = 0; i < ChildCount; ++i)
      {
          if (USRClueMapCombinedResultWidget* CombinedResultWidget = Cast<USRClueMapCombinedResultWidget>(RootCanvasPanel->GetChildAt(i)))
          {
              CombinedResultWidget->ClueMapCombinedResultClickedDelegate.RemoveDynamic(this, &ThisClass::UpdateClueMapCombinedResultDescriptionWidget);
          }
      }

}


void USRClueMapWidget::FindCombinedResultWidgets()
{
      check(RootCanvasPanel);

      const int32 ChildCount = RootCanvasPanel->GetChildrenCount();
      for (int32 i = 0; i < ChildCount; ++i)
      {
            if (USRClueMapCombinedResultWidget* CombinedResultWidget = Cast<USRClueMapCombinedResultWidget>(RootCanvasPanel->GetChildAt(i)))
            {
                  CombinedResultWidget->ClueMapCombinedResultClickedDelegate.AddDynamic(this, &ThisClass::UpdateClueMapCombinedResultDescriptionWidget);
                  ClueMapResults.Add(CombinedResultWidget);
            }
      }
      MaxClueMapNum = ClueMapResults.Num();
}

void USRClueMapWidget::FindTrueClueLinkWidgets()
{
      check(RootCanvasPanel);

      const int32 ChildCount = RootCanvasPanel->GetChildrenCount();
      for (int32 i = 0; i < ChildCount; ++i)
      {
            //찾은 Widget이 USRClueMapCombinedResultWidget인지 확인한다.
            if (USRTrueClueLinkWidget* TrueClueLinkWidget = Cast<USRTrueClueLinkWidget>(RootCanvasPanel->GetChildAt(i)))
            {
                UE_LOG(LogTemp, Warning, TEXT("TrueClueLinkWidget : %s"), *TrueClueLinkWidget->GetName());
                  TrueClueLinkWidgets.Add(TrueClueLinkWidget);
            }
      }
} 


void USRClueMapWidget::HandleTrueClueLinkWidget(FName CombinedClueId)
{

}

void USRClueMapWidget::HandleCombinedClue(const FSRClueMapUIData& Data)
{
    if (!ClueMapResults.IsValidIndex(NextFillIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("NextFillIndex is not vaild"));
    }

    USRClueMapCombinedResultWidget* ClueMapResult = ClueMapResults[NextFillIndex];
    if (!ClueMapResult)
        return;


    // ===== 아이콘 개수에 따른 슬롯 사이즈 적용 =====
    const int32 IconCount = Data.ClueIcons.Num();
    const bool bThree = (IconCount == 3);
    const FVector2D DesiredSize = bThree ? ThreeClueBasedClueMapSize : TwoClueBasedClueMapSize;

    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ClueMapResult->Slot))
    {
        CanvasSlot->SetSize(DesiredSize);  // 원하는 사이즈 적용
    }

    ClueMapResult->UpdateClueImage(Data.ClueIcons);    // 아이콘 배열, 이름 배열, 설명 등

    if (Data.ClueMap.bResult)
    {
        ClueMapResult->SetClueMapId(Data.ClueMap.Id);
    }
    ClueMapResult->SetVisibility(ESlateVisibility::Visible);

    CachedUIByClueId.Add(Data.ClueMap.Id, Data);

      
    //진실 단서라면
    if (Data.bResult == true)
    {
        // 예) TrueCount에 대응하는 인덱스의 링크만 표시
        if (TrueClueLinkWidgets.IsValidIndex(NextFillIndex) && TrueClueLinkWidgets[NextFillIndex])
        {
            TrueClueLinkWidgets[NextFillIndex]->SetVisibility(ESlateVisibility::Visible);
        }       

            TrueClues.Add(Data.ClueMap.Id);

            const float Ratio = GetProgressRatio();

            UpdateClueMapProgressBar(Ratio);
            UpdatePercentTextBlock(Ratio);
    }
    else
    {
        if (TrueClueLinkWidgets.IsValidIndex(NextFillIndex) && TrueClueLinkWidgets[NextFillIndex])
        {
            TrueClueLinkWidgets[NextFillIndex]->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    ++NextFillIndex;

}

void USRClueMapWidget::UpdateClueMapProgressBar(float NewPercent)
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateClueMap Progressbar (0~1): %f"), NewPercent);
    
    ClueMapProgressBar->SetPercent(FMath::Clamp(NewPercent, 0.f, 1.f));
    
}

void USRClueMapWidget::UpdatePercentTextBlock(float NewPercent)
{
   
    // NewPercent는 0~1 비율. AsPercent가 자동으로 0~100% 표시
    FNumberFormattingOptions Opt;
    Opt.MaximumFractionalDigits = 2; // 소수 없애려면 0, 원하면 1~2로
    PercentTextBlock->SetText(FText::AsPercent(FMath::Clamp(NewPercent, 0.f, 1.f), &Opt));
}

void USRClueMapWidget::UpdateClueMapCombinedResultDescriptionWidget(const FName& Id)
{
    const FSRClueMapUIData* Data = CachedUIByClueId.Find(Id);

    if (!Data)
        return;

    // 아이콘 2/3 세팅
    ClueMapCombinedDesWidget->SetClueIcons(Data->ClueIcons);

   
    ClueMapCombinedDesWidget->SetClueNamesText(Data->ClueNames);

    // 조합 결과의 타이틀/설명
    ClueMapCombinedDesWidget->SetClueMapName(Data->ClueMap.Name);
    ClueMapCombinedDesWidget->SetClueMapDes(Data->ClueMap.Description);

    ClueMapCombinedDesWidget->SetVisibility(ESlateVisibility::Visible);
}

void USRClueMapWidget::InitializeSlots()
{
}

void USRClueMapWidget::InitializeLinks()
{
}

void USRClueMapWidget::ResetClueMapUI()
{
}

float USRClueMapWidget::GetProgressRatio() const
{
    if (MaxTrueClueMapNum <= 0) return 0.f;
    const float Ratio =  static_cast<float>(TrueClues.Num()) / static_cast<float>(MaxTrueClueMapNum);
    UE_LOG(LogTemp, Warning, TEXT("GetProgressRatio : %f"), Ratio);
    return FMath::Clamp(Ratio, 0.f, 1.f);
}

void USRClueMapWidget::FindPlayerInventoryComponent()
{
      if (!InventoryComponent)
      {
            if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
            {
                  if (APawn* Pawn = PC->GetPawn())
                  {
                        InventoryComponent = Pawn->FindComponentByClass<USRInventoryComponent>();
                  }
            }
      }
}

void USRClueMapWidget::BindInventoryDelegate()
{
      if (!InventoryComponent)
      {
            UE_LOG(LogTemp, Warning, TEXT("ClueMapWidget - Inventory Widget Not Found"));
            return;
      }

      InventoryComponent->ClueMapResultDelegate.AddDynamic(this, &ThisClass::HandleCombinedClue);
}

void USRClueMapWidget::UnbindInventoryDelegate()
{
      if (!InventoryComponent)
      {
            UE_LOG(LogTemp, Warning, TEXT("ClueMapWidget - Inventory Widget Not Found"));
            return;
      }

      InventoryComponent->ClueMapResultDelegate.RemoveDynamic(this, &ThisClass::HandleCombinedClue);
}

