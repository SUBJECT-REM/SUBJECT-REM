// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Caption/SRCaptionWidget.h"
#include "Components/RichTextBlock.h"
#include "SRItemData.h"
#include "Engine/DataTable.h"

void USRCaptionWidget::ShowCaptionRow(FName Row)
{
	if (!CaptionDataTable)
	{
		UE_LOG(LogTemp,Warning,TEXT(""))
		return;
	}
	FString Ctx;
	FSRCaptionData* FoundData = CaptionDataTable->FindRow<FSRCaptionData>(Row, Ctx);

	if (!FoundData)
	{
		return;
	}
}

void USRCaptionWidget::ClearTextBlock()
{

}
