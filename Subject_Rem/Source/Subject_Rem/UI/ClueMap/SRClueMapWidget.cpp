// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Component/SRInventoryComponent.h"
#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "UI/ClueMap/SRTrueClueLinkWidget.h"

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
}


void USRClueMapWidget::FindCombinedResultWidgets()
{
      check(ClueMapCanvas);

      const int32 ChildCount = ClueMapCanvas->GetChildrenCount();
      for (int32 i = 0; i < ChildCount; ++i)
      {
            //찾은 Widget이 USRClueMapCombinedResultWidget인지 확인한다.
            if (USRClueMapCombinedResultWidget* CombinedResultWidget = Cast<USRClueMapCombinedResultWidget>(ClueMapCanvas->GetChildAt(i)))
            {
                  //UE_LOG(LogTemp, Warning, TEXT("Found ClueMapCombined ID : %s"), *CombinedResultWidget->CombinedClueID.ToString());
                  CombinedClueWidgets.Add(CombinedResultWidget->CombinedClueID, CombinedResultWidget);

                  //Test용으로 처음부터 더해버림 이후에 삭제해야함.
                  //TrueClues.Add(CombinedResultWidget->CombinedClueID);
                  ////Test로 10초 뒤 그림
                  //FTimerHandle TestTimerHandle;
                  //GetWorld()->GetTimerManager().SetTimer(TestTimerHandle,this,&USRClueMapWidget::DrawTrueClueLinkLine,10.0f, false);
            }
      }
}

void USRClueMapWidget::FindTrueClueLinkWidgets()
{
      check(ClueLinkLineCanvas);

      const int32 ChildCount = ClueLinkLineCanvas->GetChildrenCount();
      for (int32 i = 0; i < ChildCount; ++i)
      {
            //찾은 Widget이 USRClueMapCombinedResultWidget인지 확인한다.
            if (USRTrueClueLinkWidget* TrueClueLinkWidget = Cast<USRTrueClueLinkWidget>(ClueLinkLineCanvas->GetChildAt(i)))
            {
                  TrueClueLinkWidgets.Add(TrueClueLinkWidget);
            }
      }
} 

void USRClueMapWidget::FindCombinedClueResultWidget(FName CombinedClueID)
{
      for (const TPair<FName, USRClueMapCombinedResultWidget*>& Pair : CombinedClueWidgets)
      {
            FName Key = Pair.Key;
            USRClueMapCombinedResultWidget* Widget = Pair.Value;

            //해당 Widget을 찾았다면 
            if (Key == CombinedClueID)
            {
                  FoundCombinedClueWidgets.Add(Key, Widget);
                  Widget->SetVisibility(ESlateVisibility::Visible);
            }
      }
}

void USRClueMapWidget::HandleCombinedClue(const FSRClueMapData& Data)
{
      //여기서 Broadcast로 들어온 Data에서 FName을 추출한다.
      FSRItemBaseData CombinedClueBaseInfo= Data.BaseInfo;
      FName CombinedClueID = CombinedClueBaseInfo.Id;

      UE_LOG(LogTemp, Warning, TEXT("CombinedClueID: %s"), *CombinedClueID.ToString());
      FindCombinedClueResultWidget(CombinedClueID);

      //진실 단서라면
      if (Data.bResult == true)
      {
            UE_LOG(LogTemp, Warning, TEXT("True Clue Combined"));
            TrueClues.Add(CombinedClueID);
      }
}

void USRClueMapWidget::DrawTrueClueLinkLine()
{
      for (USRTrueClueLinkWidget* LinkLine : TrueClueLinkWidgets)
      {
            if (!LinkLine) continue;

            //해당 LinkWidget이 연결될 두 단서가 현재 찾아진 단서인지 확인합니다.
            const bool bClueAFound = TrueClues.Contains(LinkLine->GetClueAName());
            const bool bClueBFound = TrueClues.Contains(LinkLine->GetClueBName());

            //둘 다 찾은 단서라면 해당 두 단서를 연결하는 Widget을 보이도록 합니다.
            if (bClueAFound && bClueBFound)
            {
                  LinkLine->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                  LinkLine->SetVisibility(ESlateVisibility::Hidden);
            }
      }
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

