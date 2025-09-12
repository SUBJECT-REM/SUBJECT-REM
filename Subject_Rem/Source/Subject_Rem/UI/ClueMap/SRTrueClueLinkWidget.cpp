// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClueMap/SRTrueClueLinkWidget.h"
#include "Components/Button.h"
void USRTrueClueLinkWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (LinkButtonImage && NormalImage && HoverImage)
	{
		// Brush 생성
		FSlateBrush NormalBrush;
		NormalBrush.SetResourceObject(NormalImage);

		FSlateBrush HoverBrush;
		HoverBrush.SetResourceObject(HoverImage);

		// 버튼 스타일 구성
		FButtonStyle Style;
		Style.SetNormal(NormalBrush);   // 기본 상태
		Style.SetHovered(HoverBrush);   // 마우스 오버
		Style.SetPressed(HoverBrush);   // 눌렀을 때도 Hover 이미지 사용
		Style.SetDisabled(NormalBrush); // 비활성화 시에도 Normal 사용

		LinkButtonImage->SetStyle(Style);
	}
}

void USRTrueClueLinkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//해당 위젯을 안보이게 설정
	SetVisibility(ESlateVisibility::Hidden);
}
