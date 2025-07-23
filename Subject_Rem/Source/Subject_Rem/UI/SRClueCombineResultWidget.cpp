// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRClueCombineResultWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"

void USRClueCombineResultWidget::SetClueMapImage(UTexture2D* Icon)
{
	ClueMapImage->SetBrushFromTexture(Icon);
	
}

void USRClueCombineResultWidget::SetClueMapDes(FName Text)
{
	ClueMapDes->SetText(FText::FromName(Text));
}

void USRClueCombineResultWidget::SetClueMapName(FName Text)
{
	ClueMapName->SetText(FText::FromName(Text));
}

void USRClueCombineResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ClueMapImage->SetVisibility(ESlateVisibility::HitTestInvisible);

	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseButton);
}

void USRClueCombineResultWidget::OnClickedCloseButton()
{
	UE_LOG(LogTemp, Warning, TEXT("USRClueCombineResultWidget :: OnClicked Close Buton"));
	SetVisibility(ESlateVisibility::Collapsed);
}
