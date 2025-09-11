// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRCluemapCombinedDesWidget.h"
#include"Components/Image.h"
#include "Components/RichTextBlock.h"
#include"Components/Button.h"

void USRCluemapCombinedDesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseButtonClicked);
}

void USRCluemapCombinedDesWidget::SetLeftRightImage(TSoftObjectPtr<UTexture2D> Left, TSoftObjectPtr<UTexture2D> Right)
{
	if (Left.IsNull() || Right.IsNull())
		return;

	LeftImage->SetBrushFromSoftTexture(Left);
	RightImage->SetBrushFromSoftTexture(Right);
}

void USRCluemapCombinedDesWidget::SetLeftRightItemName(FName Left, FName Right)
{
	LeftImageItemName->SetText(FText::FromName(Left));
	RightImageItemName->SetText(FText::FromName(Right));
}

void USRCluemapCombinedDesWidget::SetClueMapName(FName Name)
{
	ClueMapName->SetText(FText::FromName(Name));
}

void USRCluemapCombinedDesWidget::SetClueMapDes(FText Des)
{
	ClueMapDes->SetText(Des);
}

void USRCluemapCombinedDesWidget::OnCloseButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
}
