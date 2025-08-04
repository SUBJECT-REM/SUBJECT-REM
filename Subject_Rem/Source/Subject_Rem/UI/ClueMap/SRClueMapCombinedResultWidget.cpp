// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "Components/Image.h"
#include "SRItem.h"

void USRClueMapCombinedResultWidget::NativeConstruct()
{
      //이후 Hidden으로 변경
      //SetVisibility(ESlateVisibility::Visible);
      
      SetVisibility(ESlateVisibility::Hidden);
     

      //CombinedCLueID로 이미지를 Load해야합니다.

      //if (ClueDataTable && CombinedClueImage)
      //{
      //      // RowName으로 DataTable Row 검색 -> A12 이런 식으로 되어있음.
      //      const FSRClueMapData* Row = ClueDataTable->FindRow<FSRClueMapData>(CombinedClueID, TEXT(""));

      //      if (Row)
      //      {
      //            //UE_LOG(LogTemp, Warning, TEXT("Struct Find Complete"));
      //            // UImage는 Texture2D를 직접 못 쓰므로 Brush로 변환
      //            FSlateBrush Brush;
      //            //Brush.SetResourceObject(Row->CharacterTexture);
      //            CombinedClueImage->SetBrush(Brush);
      //      }
      //}
}
