// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRTrueClueLinkWidget.h"

void USRTrueClueLinkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//해당 위젯을 안보이게 설정
	SetVisibility(ESlateVisibility::Hidden);
}
