// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Component/SRInventoryComponent.h"
#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"

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
                  UE_LOG(LogTemp, Warning, TEXT("Found ClueMapCombined ID : %s"), *CombinedResultWidget->CombinedClueID.ToString());
                  CombinedClueWidgets.Add(CombinedResultWidget->CombinedClueID, CombinedResultWidget);
            }
      }
}

void USRClueMapWidget::FindCombinedClueResultWidget(FName CombinedClueName)
{
      UE_LOG(LogTemp, Warning, TEXT("FindCombinedClueResultWidget : %s"), *CombinedClueName.ToString());
      for (const TPair<FName, USRClueMapCombinedResultWidget*>& Pair : CombinedClueWidgets)
      {
            FName Key = Pair.Key;
            USRClueMapCombinedResultWidget* Widget = Pair.Value;

            //해당 Widget을 찾았다면 
            if (Key == CombinedClueName)
            {
                  UE_LOG(LogTemp, Warning, TEXT("Clue Found"));
                  FoundCombinedClueWidgets.Add(Key, Widget);
                  Widget->SetVisibility(ESlateVisibility::Visible);
            }
      }
}

void USRClueMapWidget::HandleCombinedClue(const FSRClueMapData& Data)
{
      //여기서 Broadcast로 들어온 Data에서 FName을 추출한다.
      UE_LOG(LogTemp, Warning, TEXT("Clue Combined"));
      FSRItemBaseData CombinedClueBaseInfo= Data.BaseInfo;
      FName CombinedClueName = CombinedClueBaseInfo.Name;

      FindCombinedClueResultWidget(CombinedClueName);

      //진실 단서라면
      if (Data.bResult == true)
      {
            UE_LOG(LogTemp, Warning, TEXT("True Clue Combined"));
            TrueClues.Add(CombinedClueName);
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

