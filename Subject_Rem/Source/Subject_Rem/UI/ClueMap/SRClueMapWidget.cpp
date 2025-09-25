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
              CombinedClueWidgets.Add(CombinedResultWidget->CombinedClueID, CombinedResultWidget);
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
                  CombinedClueWidgets.Add(CombinedResultWidget->CombinedClueID, CombinedResultWidget);
                  CombinedResultWidget->ClueMapCombinedResultClickedDelegate.AddDynamic(this, &ThisClass::UpdateClueMapCombinedResultDescriptionWidget);
            }
      }
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
                  TrueClueLinkWidgets.Add(TrueClueLinkWidget->CombinedClueID,TrueClueLinkWidget);
            }
      }
} 

USRClueMapCombinedResultWidget* USRClueMapWidget::FindCombinedClueResultWidget(FName CombinedClueID)
{
      for (const TPair<FName, USRClueMapCombinedResultWidget*>& Pair : CombinedClueWidgets)
      {
            FName Key = Pair.Key;
            USRClueMapCombinedResultWidget* Widget = Pair.Value;

            //해당 Widget을 찾았다면 
            if (Key == CombinedClueID)
            {
                  Widget->SetVisibility(ESlateVisibility::Visible);
            
                  return Widget;
            }
            
      }

      return nullptr;
}

void USRClueMapWidget::HandleTrueClueLinkWidget(FName CombinedClueId)
{
    for (const TPair<FName, USRTrueClueLinkWidget*>& Pair : TrueClueLinkWidgets)
    {
        FName Key = Pair.Key;
        USRTrueClueLinkWidget* Widget = Pair.Value;

        //해당 Widget을 찾았다면 
        if (Key == CombinedClueId)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);
        }

    }
}

void USRClueMapWidget::HandleCombinedClue(const FSRClueMapUIData& Data)
{
      //여기서 Broadcast로 들어온 Data에서 FName을 추출한다.
      FName CombinedClueID = Data.ClueMap.Id;

      UE_LOG(LogTemp, Warning, TEXT("CombinedClueID: %s"), *CombinedClueID.ToString());
      USRClueMapCombinedResultWidget* FindWidget = FindCombinedClueResultWidget(CombinedClueID);
      HandleTrueClueLinkWidget(CombinedClueID);
      //진실 단서라면
      if (Data.bResult == true)
      {
            UE_LOG(LogTemp, Warning, TEXT("True Clue Combined"));
            
            //WidgetImageSetting
            if (FindWidget)
            {
                FindWidget->UpdateLeftRightClueImage(nullptr,nullptr);
            }
            
            TrueClues.Add(CombinedClueID);

            const float Ratio = GetProgressRatio();

            UpdateClueMapProgressBar(Ratio);
            UpdatePercentTextBlock(Ratio);
      }
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

void USRClueMapWidget::UpdateClueMapCombinedResultDescriptionWidget(const FSRClueMapData& Data)
{

    //ClueMapCombinedDesWidget->SetLeftRightImage(Data.LeftIcon,Data.RightIcon);
    ClueMapCombinedDesWidget->SetClueMapName(Data.Name);
    //ClueMapCombinedDesWidget->SetLeftRightItemName(Data.LeftIconItemName,Data.RightIconItemName);
    ClueMapCombinedDesWidget->SetClueMapDes(Data.Description);

    //TODO : ClueMapCombinedDesWidget에서 직접 Visibility처리하도록 할것
    ClueMapCombinedDesWidget->SetVisibility(ESlateVisibility::Visible);
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

      InventoryComponent->ClueCombineResultDelegate.AddDynamic(this, &ThisClass::HandleCombinedClue);
}

void USRClueMapWidget::UnbindInventoryDelegate()
{
      if (!InventoryComponent)
      {
            UE_LOG(LogTemp, Warning, TEXT("ClueMapWidget - Inventory Widget Not Found"));
            return;
      }

      InventoryComponent->ClueCombineResultDelegate.RemoveDynamic(this, &ThisClass::HandleCombinedClue);
}

