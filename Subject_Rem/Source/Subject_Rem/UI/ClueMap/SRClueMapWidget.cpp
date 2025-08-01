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
}

void USRClueMapWidget::NativeConstruct()
{
      Super::NativeConstruct();

	if (!bIsInitialized)
	{
		bIsInitialized = true;
            //최초 한번만 실행할 로직
            
	}
	
      //Inventory와 Bind
      
}

void USRClueMapWidget::NativeDestruct()
{
      Super::NativeDestruct();
      UnbindInventoryDelegate(); // 닫을 때 바인딩 해제
}


void USRClueMapWidget::FindClueCombinedResultWidgets()
{
      check(ClueMapCanvas);

      const int32 ChildCount = ClueMapCanvas->GetChildrenCount();
      for (int32 i = 0; i < ChildCount; ++i)
      {
            //찾은 Widget이 USRClueMapCombinedResultWidget인지 확인한다.
            if (USRClueMapCombinedResultWidget* CombinedResultWidget = Cast<USRClueMapCombinedResultWidget>(ClueMapCanvas->GetChildAt(i)))
            {
                  //CombinedResultWidget
            }
      }
}

void USRClueMapWidget::HandleCombinedClue(const FSRClueMapData& Data)
{
      //여기서 Broadcast로 들어온 Data에서 FName을 추출한다.
      FSRItemBaseData CombinedClueBaseInfo= Data.BaseInfo;
      FName CombinedClueName = CombinedClueBaseInfo.Name;
      CombinedClueWidgets.Add(CombinedClueName, Data);

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
      //찾은 InventoryComponent가 없으면 찾는다.
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

