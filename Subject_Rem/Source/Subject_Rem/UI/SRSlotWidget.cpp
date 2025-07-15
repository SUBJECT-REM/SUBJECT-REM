// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRSlotWidget.h"
#include "Components/Button.h"

void USRSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	}
}

void USRSlotWidget::SetSlotStyle(UObject* Icon)
{
	check(Button)

	if (Icon == nullptr)
	{
		Button->SetStyle(FButtonStyle::GetDefault());
	}

	else
	{
		FButtonStyle ButtonStyle;
		FSlateBrush NormalStyle;
		NormalStyle.SetResourceObject(Icon);
		ButtonStyle.SetNormal(NormalStyle);

		FSlateBrush HoverStyle;
		HoverStyle.SetResourceObject(Icon);
		ButtonStyle.SetHovered(HoverStyle);

		FSlateBrush PressStyle;
		PressStyle.SetResourceObject(Icon);
		ButtonStyle.SetPressed(PressStyle);

		Button->SetStyle(ButtonStyle);
	}
}

void USRSlotWidget::SetClueData(const FSRClueData& NewData)
{
	ClueData = NewData;
}

const FSRClueData& USRSlotWidget::GetClueData()
{
	return ClueData;
}

void USRSlotWidget::OnButtonClicked()
{
	FOnSlotClickedDelegate.Broadcast(this);
}
