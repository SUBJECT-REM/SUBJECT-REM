// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRClueMapCombinedResultWidget.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/Button.h"

void USRClueMapCombinedResultWidget::NativeConstruct()
{
     
      SetVisibility(ESlateVisibility::Visible);
     
      ClueMapDesShownButton->OnClicked.AddDynamic(this, &ThisClass::OnCobminedClueButtonClicked);
      LeftClueImage->SetVisibility(ESlateVisibility::Hidden);
      RightClueImage->SetVisibility(ESlateVisibility::Hidden);
}

void USRClueMapCombinedResultWidget::UpdateLeftRightClueImage(TSoftObjectPtr<UTexture2D> Left, TSoftObjectPtr<UTexture2D> Right)
{
    LeftClueImage->SetBrushFromSoftTexture(Left);
    RightClueImage->SetBrushFromSoftTexture(Right);

    LeftClueImage->SetVisibility(ESlateVisibility::Visible);
    RightClueImage->SetVisibility(ESlateVisibility::Visible);
}

void USRClueMapCombinedResultWidget::OnCobminedClueButtonClicked()
{
    //Id를 통해 ClueMapData를 가져옴
    FString Ctx;
    FSRClueMapData* FoundRow = ClueDataTable->FindRow<FSRClueMapData>(CombinedClueID, Ctx);

    if(FoundRow)
    {
        ClueMapCombinedResultClickedDelegate.Broadcast(*FoundRow);
    }

    //델리게이트로 넘김 
}
